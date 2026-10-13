#include "translatabletreeview.h"

#include <QApplication>
#include <QComboBox>
#include <QHash>
#include <QHeaderView>
#include <QStandardItemModel>
#include <QTranslator>
#include <QVBoxLayout>

namespace {

class EnTranslator : public QTranslator
{
public:
    bool isEmpty() const override { return false; }

    QString translate(const char *, const char *text, const char *, int) const override
    {
        return m_dict.value(QString::fromUtf8(text));
    }

private:
    const QHash<QString, QString> m_dict = {
        {"Параметр", "Parameter"},
        {"Переданные значения", "Transmitted values"},
        {"Значения", "Values"},
        {"Подадрес", "Subaddress"},
        {"Крен объекта", "Vehicle roll"},
        {"Тангаж", "Pitch"},
        {"Курс гирополукомпасный", "Directional gyro heading"},
        {"Признаки достоверности данных", "Data validity flags"},
        {"Резерв", "Reserved"},
        {"Достоверность курса гирополукомпасного", "Directional gyro heading validity"},
        {"Достоверность тангажа", "Pitch validity"},
        {"Достоверность крена объекта", "Vehicle roll validity"},
    };
};

QStandardItem *addRow(QStandardItem *parent, const QString &name,
                      const QString &raw = {}, const QString &value = {})
{
    auto *item = new QStandardItem(name);
    parent->appendRow({item, new QStandardItem(raw), new QStandardItem(value)});
    return item;
}

}

int main(int argc, char *argv[])
{
    QApplication app(argc, argv);
    EnTranslator translator;

    QStandardItemModel model;
    model.setHorizontalHeaderLabels({"Параметр", "Переданные значения", "Значения"});

    auto *sub = addRow(model.invisibleRootItem(), "Подадрес");
    addRow(sub, "Крен объекта", "0x0", "0.00 °");
    addRow(sub, "Тангаж", "0x0", "0.00 °");
    addRow(sub, "Курс гирополукомпасный", "0x0", "0.00 °");

    auto *flags = addRow(sub, "Признаки достоверности данных", "0x0");
    addRow(flags, "Резерв", "0x0");
    addRow(flags, "Достоверность курса гирополукомпасного", "", "0");
    addRow(flags, "Достоверность тангажа", "", "0");
    addRow(flags, "Достоверность крена объекта", "", "0");

    auto *view = new TranslatableTreeView;
    view->setModel(&model);
    view->expandAll();
    view->header()->setSectionResizeMode(QHeaderView::ResizeToContents);

    auto *combo = new QComboBox;
    combo->addItems({"Русский", "English"});
    QObject::connect(combo, qOverload<int>(&QComboBox::currentIndexChanged), [&translator](int i) {
        if (i == 1)
            QApplication::installTranslator(&translator);
        else
            QApplication::removeTranslator(&translator);
    });

    QWidget w;
    auto *layout = new QVBoxLayout(&w);
    layout->addWidget(combo, 0, Qt::AlignRight);
    layout->addWidget(view);
    w.resize(640, 340);
    w.show();

    return app.exec();
}
