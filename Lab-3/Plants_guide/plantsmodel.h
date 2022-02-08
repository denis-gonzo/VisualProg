#pragma once

#include <QAbstractItemModel>

#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonArray>
#include <QJsonValue>


class PlantsModel : public QAbstractItemModel
{
    Q_OBJECT

public:
    explicit PlantsModel(QObject *parent = nullptr);
    ~PlantsModel();

    void loadFromJson(QByteArray &jsonData);
    QByteArray saveToJson();

    QModelIndex index(int row, int column, const QModelIndex &parent) const override;
    QModelIndex parent(const QModelIndex &child) const override;

    int rowCount(const QModelIndex &parent = QModelIndex()) const override;
    int columnCount(const QModelIndex &parent = QModelIndex()) const override;

    QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const override;
    bool setData(const QModelIndex &index, const QVariant &value, int role = Qt::EditRole) override;

    Qt::ItemFlags flags(const QModelIndex &index) const override;

    bool hasChildren(const QModelIndex &parent) const override;

    bool insertRows(int position, int rows,
                    const QModelIndex &parent = QModelIndex()) override;
    bool removeRows(int position, int rows,
                    const QModelIndex &parent = QModelIndex()) override;

    QStringList availableParents(const QModelIndex &index) const;

    void clearModel();

    QStringList parentList(const QModelIndex &index) const;
    int level(const QModelIndex &index) const;

    void cutItem(const QModelIndex &index);
    void pasteItem(const QModelIndex &parent);
    bool hasCut() { return hasCuttedNode; }

private:
    struct PlantNode
    {
        PlantNode(PlantNode* p = nullptr):
            parentNode{p}
        {
            if(p)
                p->children.append(this);
        }
        ~PlantNode()
        {
            for(auto c : children)
                delete c;
        }

        QString name;
        QString info;
        QString imageData;

        PlantNode *parentNode;
        QList<PlantNode*> children;

    };

    PlantNode* rootNode;

    enum Columns
    {
        BranchColumn,
        NameColumn = BranchColumn,
        InfoColumn,
        ImageDataColumn,
        ColumnCount
    };

    int childNumber(const PlantNode* node) const;

    void parseJsonArray(const QJsonArray &jarray, PlantNode* p);
    void createJsonObject(QJsonObject &jobj, PlantNode* p);

    int nodeLevel(const PlantNode *node) const;

    PlantNode* cuttedNode;
    bool hasCuttedNode;
};

