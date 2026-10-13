#include "translatabletreeview.h"

#include <QEvent>
#include <QIdentityProxyModel>

namespace {

QVariant translated(const QVariant &v, int role)
{
    if (role != Qt::DisplayRole || v.userType() != QMetaType::QString)
        return v;

    return TranslatableTreeView::tr(v.toString().toUtf8().constData());
}

class TranslationProxy : public QIdentityProxyModel
{
public:
    using QIdentityProxyModel::QIdentityProxyModel;

    QVariant data(const QModelIndex &index, int role) const override
    {
        return translated(QIdentityProxyModel::data(index, role), role);
    }

    QVariant headerData(int section, Qt::Orientation orientation, int role) const override
    {
        return translated(QIdentityProxyModel::headerData(section, orientation, role), role);
    }
};

}

TranslatableTreeView::TranslatableTreeView(QWidget *parent)
    : QTreeView(parent)
    , m_proxy(new TranslationProxy(this))
{
    QTreeView::setModel(m_proxy);
    setEditTriggers(NoEditTriggers);
}

void TranslatableTreeView::setModel(QAbstractItemModel *model)
{
    m_proxy->setSourceModel(model);
}

void TranslatableTreeView::changeEvent(QEvent *e)
{
    if (e->type() == QEvent::LanguageChange)
        scheduleDelayedItemsLayout();

    QTreeView::changeEvent(e);
}
