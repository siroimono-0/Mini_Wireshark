#pragma once
#include <QString>
#include <QVector>

class TreeModel;

// 트리 구조의 하나의 노드
class TreeItem {

    friend TreeModel;
public:
    // title: 표시 문자열, p: 부모 노드
    TreeItem(const QString& title, TreeItem* p = nullptr);

    ~TreeItem();

    // 자식 추가
    void addChild(TreeItem* child);

    // row 번째 자식 반환
    TreeItem* child(int row);

    // 자식 개수
    int childCount() const;

    // 부모의 children 중 내 index
    int row() const;

private:
    QString s_title;                // 표시 문자열
    QVector<TreeItem*> children;    // 자식 노드 목록
    TreeItem* parent = nullptr;     // 부모 노드
};
