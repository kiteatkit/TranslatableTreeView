#pragma once

#include <QTreeView>

class QIdentityProxyModel;

class TranslatableTreeView : public QTreeView
{
    Q_OBJECT

public:
    explicit TranslatableTreeView(QWidget *parent = nullptr);

    void setModel(QAbstractItemModel *model) override;

protected:
    void changeEvent(QEvent *e) override;

private:
    QIdentityProxyModel *m_proxy;
};
