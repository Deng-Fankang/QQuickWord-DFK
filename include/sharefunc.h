#pragma once
#include "qutreemodel.h"
#include "worddefine.h"

extern TreeItem* GetTitleTreeFromNode(const TitleContentNode* root_title);
extern void DeleteTitleTree(TreeItem* root);
extern TitleContentNode* GetTitleContentNode(TitleContentNode* node, TitleContentNode* parent = nullptr);
//12