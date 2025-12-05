#pragma once
#include <QAbstractItemModel>
#include "TreeItem.h"

class TreeItem;

// 트리 구조를 QML TreeView에서 쓸 수 있게 제공하는 모델
class TreeModel : public QAbstractItemModel {
    Q_OBJECT
    friend TreeItem;

public:

    TreeItem* root = nullptr;   // 트리의 최상위 루트 노드 (UI에는 표시되지 않음)

    // 생성자 / 소멸자
    explicit TreeModel(QObject *parent = nullptr);
    ~TreeModel();

    // -------------------------------------------------------------
    // 필수 오버라이드: TreeView가 트리 구조를 탐색할 때 호출됨
    // -------------------------------------------------------------

    // row, col 위치의 노드 → QModelIndex로 변환
    QModelIndex index(int row, int col,
                      const QModelIndex& parent = QModelIndex()) const override;

    // child index의 부모 노드를 반환
    QModelIndex parent(const QModelIndex& child) const override;

    // 특정 parent 아래의 자식 개수 반환
    int rowCount(const QModelIndex& parent = QModelIndex()) const override;

    // 트리의 column 개수 (보통 1)
    int columnCount(const QModelIndex& parent = QModelIndex()) const override;

    // index가 가리키는 노드의 표시 데이터 반환
    QVariant data(const QModelIndex& index,
                  int role = Qt::DisplayRole) const override;

    // QML에서 사용할 role 이름 정의
    QHash<int, QByteArray> roleNames() const override;

    // -------------------------------------------------------------
    // 유틸 함수: 외부에서 트리를 교체/초기화할 때 사용
    // -------------------------------------------------------------

    // 트리의 초기 루트 설정 // 전체 트리를 새 root로 교체
    void setPacketTree(TreeItem* newRoot);

    // 루트를 비우고 트리 초기화
    void clear();
};
