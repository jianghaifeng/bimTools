#pragma once

#include <vector>
#include <string>

using namespace std;

class FileNode {
public:
    string name;
    string path;
    bool isFolder;
    vector<FileNode> subNodes;

public:
    FileNode(string name, string path, bool isFolder = false) {
        this->name = name;
        this->path = path;
        this->isFolder = isFolder;
        this->subNodes.clear();
    }
};

class ProjectEntity
{
public:
    string projectId;
    string folderId;
    FileNode* root;

    ProjectEntity(string projectId, string folderId, string folderName);
    ~ProjectEntity();

    FileNode* addFileNode(FileNode* parent, string name, string dir, bool isFolder = false);
    string toJsonString();
    string toJsonString(FileNode node);
};

