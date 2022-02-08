#include "plantsmodel.h"

PlantsModel::PlantsModel(QObject *parent) :
    QAbstractItemModel(parent),
    rootNode{nullptr},
    hasCuttedNode{false}
{

    rootNode = new PlantNode();
    rootNode->name = "Эукариоты";
    PlantNode* subroot = new PlantNode(rootNode);
    subroot->name = "Растения";

}
PlantsModel::~PlantsModel()
{
    if(rootNode)
        delete rootNode;
}
QModelIndex PlantsModel::index(int row, int column, const QModelIndex &parent) const
{
    if(!hasIndex(row,column, parent))
        return QModelIndex{};

    if(!parent.isValid()){
        return createIndex(0, column, rootNode);
    }

    PlantNode* pNode = static_cast<PlantNode*>(parent.internalPointer());
    if( row < 0 || row >= pNode->children.size() ){
        return QModelIndex{};
    }

    return createIndex(row, column, pNode->children.at(row));
}
QModelIndex PlantsModel::parent(const QModelIndex &child) const
{
    if(!child.isValid())
        return QModelIndex{};

    const PlantNode* childNode = static_cast<PlantNode*>(child.internalPointer());
    PlantNode* pNode = childNode->parentNode;

    if( pNode )
        return createIndex(childNumber(pNode), BranchColumn, pNode);
    else
        return QModelIndex{};
}

int PlantsModel::rowCount(const QModelIndex &parent) const
{
    if(!parent.isValid())
        return 1;

    const PlantNode* pNode = static_cast<PlantNode*>(parent.internalPointer());

    return pNode->children.size();

}
int PlantsModel::columnCount(const QModelIndex &/*parent */) const
{
    return ColumnCount;
}

QVariant PlantsModel::data(const QModelIndex &index, int role) const
{
    if(!index.isValid())
        return QVariant{};

    const PlantNode* node = static_cast<PlantNode*>(index.internalPointer());

    if(role != Qt::DisplayRole && role != Qt::EditRole) {
        return QVariant{};
    }

    switch (index.column()) {
    case NameColumn:
        return node->name;
        break;
    case InfoColumn:
        return node->info;
        break;
    case ImageDataColumn:
        return node->imageData;
        break;
    default:
        break;
    }

    return QVariant{};
}
bool PlantsModel::setData(const QModelIndex &index, const QVariant &value, int role)
{
    if(!index.isValid())
        return false;

    if(role != Qt::EditRole)
        return false;

    PlantNode* node = static_cast<PlantNode*>(index.internalPointer());
    switch (index.column()) {
    case NameColumn:
        node->name = value.toString();
        break;
    case InfoColumn:
        node->info = value.toString();
        break;
    case ImageDataColumn:
        node->imageData = value.toByteArray();
    default:
        break;
    }
    emit dataChanged(index,index.sibling(index.row(),ColumnCount));

    return true;
}

Qt::ItemFlags PlantsModel::flags(const QModelIndex &index) const
{
    Qt::ItemFlags flags = QAbstractItemModel::flags(index);
    if (index.isValid() && index.column() < ColumnCount ) {
        flags |= Qt::ItemIsDragEnabled | Qt::ItemIsDropEnabled | Qt::ItemIsEditable;
    }
    return flags;
}

bool PlantsModel::hasChildren(const QModelIndex &parent) const
{
    if(!parent.isValid())
        return QAbstractItemModel::hasChildren(parent);

    const PlantNode* pNode = static_cast<PlantNode*>(parent.internalPointer());
    return  !pNode->children.empty();

}
int PlantsModel::childNumber(const PlantNode* node) const
{
    if(node) {
        PlantNode* pNode = node->parentNode;
        if(pNode) {
            for(int i=0; i<pNode->children.size(); i++){
                if(pNode->children.at(i) == node)
                    return i;
            }
        }
    }
    return 0;
}

void PlantsModel::loadFromJson(QByteArray &jsonData)
{
    clearModel();

    QJsonDocument jsonDoc{QJsonDocument::fromJson(jsonData)};
    QJsonArray guide = jsonDoc.array();
    QJsonObject root = guide[0].toObject();
    rootNode = new PlantNode();
    rootNode->name = root["name"].toString();
    rootNode->info = root["info"].toString();
    rootNode->imageData = root["image"].toString();
    QJsonArray next = root["child"].toArray();
    parseJsonArray(next, rootNode);
}

QByteArray PlantsModel::saveToJson()
{
    QJsonArray jarr;
    QJsonObject obj;
    createJsonObject(obj, rootNode);
    jarr.append(obj);
    QJsonDocument jDoc{jarr};
    return jDoc.toJson();
}

void PlantsModel::parseJsonArray(const QJsonArray &jarray, PlantNode* p)
{
    for(auto j : jarray)
    {
        QJsonObject jo = j.toObject();
        if( !jo.contains("name") )
            continue;
        PlantNode* node = new PlantNode(p);
        node->name = jo["name"].toString();
        node->info = jo["info"].toString();
        node->imageData = jo["image"].toString();
        if( jo.contains("child")){
            QJsonArray ja = jo["child"].toArray();
            parseJsonArray(ja, node);
        }
    }
}
void PlantsModel::createJsonObject(QJsonObject &jobj, PlantNode *node)
{
    jobj["name"]=node->name;
    jobj["info"]=node->info;
    jobj["image"]=node->imageData;
    if(node->children.size() > 0) {
        QJsonArray ja;
        for(auto n : node->children){
            QJsonObject obj;
            createJsonObject(obj, n);
            ja.append(obj);
        }
        jobj["child"] = ja;
    }
}

void PlantsModel::clearModel()
{
    beginResetModel();
    if(rootNode)
        delete rootNode;
    endResetModel();
}

QStringList PlantsModel::parentList(const QModelIndex &index) const
{
    QStringList pl{};
    if(!index.isValid())
        return pl;

    const PlantNode* node = static_cast<PlantNode*>(index.internalPointer());
    while(node->parentNode){
        node = node->parentNode;
        pl.push_front(node->name);
    }
    return pl;
}
int PlantsModel::level(const QModelIndex &index) const
{
    if(!index.isValid())
        return 0;

    const PlantNode* node = static_cast<PlantNode*>(index.internalPointer());
    return nodeLevel(node);
}

int PlantsModel::nodeLevel(const PlantNode *node) const
{
    if(!node)
        return 0;

    int lv = 0;
    while(node->parentNode){
        node = node->parentNode;
        lv++;
    }
    return lv;
}

bool PlantsModel::insertRows(int position, int rows, const QModelIndex &parent)
{
    if(!parent.isValid())
        return false;

    PlantNode* parentNode = static_cast<PlantNode*>(parent.internalPointer());
    beginInsertRows(parent, position, position + rows - 1);
    if(position >= 0 && position <= parentNode->children.size() ){
        for (int row = 0; row < rows; row++){
            PlantNode* node = new PlantNode();
            node->parentNode = parentNode;
            parentNode->children.insert(position + row, node);
            node->name = "Название";
            node->info = "Описание";
            node->imageData = "[image_data]";
        }
    }
    endInsertRows();
    return true;
}
bool PlantsModel::removeRows(int position, int rows, const QModelIndex &parent)
{
    if(!parent.isValid())
        return false;

    PlantNode* parentNode = static_cast<PlantNode*>(parent.internalPointer());
    beginRemoveRows(parent, position, position + rows - 1);
    if(position >= 0 && position + rows <= parentNode->children.size() ){
        for (int row = 0; row < rows; row++)
            delete parentNode->children.takeAt(position);
    }
    endRemoveRows();
    return true;
}

void PlantsModel::cutItem(const QModelIndex &index)
{
    if(!index.isValid())
        return;

    PlantNode* node = static_cast<PlantNode*>(index.internalPointer());
    if(node){
        cuttedNode = node;
        hasCuttedNode = true;
        beginRemoveRows(index.parent(),index.row(), 1);
        PlantNode* oldparent = cuttedNode->parentNode;
        int row = childNumber( cuttedNode );
        oldparent->children.takeAt(row);
        endRemoveRows();
    }
}

void PlantsModel::pasteItem(const QModelIndex &parent)
{
    if(!parent.isValid() || !hasCuttedNode)
        return;

    hasCuttedNode = false;
    PlantNode* parentNode = static_cast<PlantNode*>(parent.internalPointer());
    if(parentNode){
        beginInsertRows(parent,0,1);
        cuttedNode->parentNode = parentNode;
        parentNode->children.push_front(cuttedNode);
        endInsertRows();
    }
}
