#include "TreeItem.h"
// title: 표시 문자열, p: 부모 노드
TreeItem::TreeItem(const QString& title, TreeItem* p)
    : s_title(title), parent(p)
{
}

TreeItem::~TreeItem()
{
    for(auto& v : this->children)
    {
        delete v;
    }
}

// 자식 추가
void TreeItem::addChild(TreeItem* child)
{
    children.push_back(child);
    child->parent = this;
}
TreeItem* TreeItem::child(int row)
{ // row 번째 자식 반환
    return children.value(row, nullptr);
}
// 자식 개수
int TreeItem::childCount() const
{
    return children.size();
}
// 부모의 children 중 내 index
int TreeItem::row() const
{
    if (!parent) return 0;
    return parent->children.indexOf(const_cast<TreeItem*>(this));
}
