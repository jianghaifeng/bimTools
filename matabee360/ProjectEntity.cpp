#include "stdafx.h"
#include "ProjectEntity.h"

ProjectEntity::ProjectEntity(string projectId, string folderId, string folderName) {
    this->projectId = projectId;
    this->folderId = folderId;
    this->root = new FileNode(folderName, "", true);
}

ProjectEntity::~ProjectEntity(void)
{
    delete this->root;
}

FileNode* ProjectEntity::addFileNode(FileNode* parent, string name, string dir, bool isFolder) {
    string path = dir;
    if (!isFolder) {
        path += +R"(\\)";
        path += name;
    }
    FileNode node(name, path, isFolder);
    parent->subNodes.push_back(node);
    return &parent->subNodes.back();
}

string ProjectEntity::toJsonString() {
    string rootStr = "{";

    rootStr.append("\"projectId\": \"");
    rootStr.append(this->projectId);
    rootStr.append("\",\n");

    rootStr.append("\"folderId\": \"");
    rootStr.append(this->folderId);
    rootStr.append("\",\n");

    rootStr.append("\"root\": ");
    rootStr.append(toJsonString(*this->root));

    rootStr.append("\n}");
    return rootStr;
}

string ProjectEntity::toJsonString(FileNode node) {
    string itemStr = "{";

    itemStr.append("\"name\": \"");
    itemStr.append(node.name);
    itemStr.append("\",\n");

    itemStr.append("\"path\": \"");
    itemStr.append(node.path);
    itemStr.append("\",\n");

    itemStr.append("\"files\": [\n");
    if (node.subNodes.size() > 0) {
        for (auto f : node.subNodes) {
            itemStr.append(toJsonString(f));
            itemStr.append(",");
        }
        itemStr = itemStr.substr(0, itemStr.length() - 1);
    }
    itemStr.append("\n]");

    itemStr.append("\n}");

    return itemStr;
}