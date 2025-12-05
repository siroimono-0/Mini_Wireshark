#include "TreeModel.h"

TreeModel::TreeModel(QObject *parent) : QAbstractItemModel {parent}
{

}

TreeModel::~TreeModel()
{

}

QModelIndex TreeModel::index(int row, int col, const QModelIndex &parentIndex) const
{
    // 컬럼이 0 이상이 아니면 무효
    if (col != 0)
        return QModelIndex();

    // parentIndex가 유효하면 parent TreeItem, 아니면 root
    TreeItem* parentItem;

    if (!parentIndex.isValid()) {
        parentItem = root;          // 최상위 부모는 루트
    } else {
        parentItem = static_cast<TreeItem*>(parentIndex.internalPointer());
    }

    // parent의 row번째 자식 가져오기
    TreeItem* childItem = parentItem->child(row);
    if (!childItem)
        return QModelIndex();       // 자식 없으면 무효

    // QModelIndex 생성 (childItem을 포인터로 설정)
    return createIndex(row, col, childItem);
}


QModelIndex TreeModel::parent(const QModelIndex &childIndex) const
{
    // childIndex가 유효하지 않으면 부모 없음
    if (!childIndex.isValid())
        return QModelIndex();

    // childIndex가 가리키는 노드(TreeItem*)
    TreeItem* childItem = static_cast<TreeItem*>(childIndex.internalPointer());
    TreeItem* parentItem = childItem->parent;

    // 루트의 부모는 없음 → invalid 반환
    if (!parentItem || parentItem == root)
        return QModelIndex();

    // 부모의 row()를 QModelIndex로 만들어 반환
    return createIndex(parentItem->row(), 0, parentItem);
}

int TreeModel::rowCount(const QModelIndex &parentIndex) const
{
    // 루트 요청일 경우 (parentIndex가 invalid)
    // → root의 자식 개수를 반환
    if (!parentIndex.isValid())
        return root->childCount();

    // parentIndex가 가리키는 TreeItem*
    TreeItem* parentItem = static_cast<TreeItem*>(parentIndex.internalPointer());

    // 해당 parent의 자식 개수 반환
    return parentItem->childCount();
}

int TreeModel::columnCount(const QModelIndex &parent) const
{
    Q_UNUSED(parent);   // 부모와 관계없이 동일

    // 트리는 보통 1컬럼만 사용
    return 1;
}

QVariant TreeModel::data(const QModelIndex &index, int role) const
{
    // 인덱스가 유효하지 않으면 표시할 데이터 없음
    if (!index.isValid())
    {
        return QVariant();
    }

    if (role != Qt::DisplayRole)
    {
        return QVariant();
    }

    // index가 가리키는 TreeItem*
    TreeItem* item = static_cast<TreeItem*>(index.internalPointer());

    // TreeItem의 s_title을 UI로 반환
    return item->s_title;
}

QHash<int, QByteArray> TreeModel::roleNames() const
{
    QHash<int, QByteArray> roles;

    // DisplayRole → "display" 라는 이름으로 QML에 노출
    roles[Qt::DisplayRole] = "display";

    return roles;
}

void TreeModel::setPacketTree(TreeItem* newRoot)
{
    beginResetModel();          // ① 모델 전부를 리셋할 준비를 함

    if (root != nullptr)    // ② 기존 트리가 있다면 메모리 해제
        delete root;

    root = newRoot;         // ③ 모델이 사용할 새로운 루트 노드를 교체

    endResetModel();            // ④ 뷰에게 “전체 구조 변경됨” 알림 → UI 갱신
}

void TreeModel::clear()
{
    beginResetModel();          // ① 모델 전체 리셋 시작

    if (root != nullptr)    // ② 기존 트리 메모리 해제
    {
        delete root;        // ③ root 및 모든 자식 노드 완전 삭제
        root = nullptr;
    }

    endResetModel();            // ④ 모델 리셋 완료 → UI 갱신
}

