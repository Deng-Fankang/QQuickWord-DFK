#pragma once
#include "qutreemodel.h"
#include "worddefine.h"

extern TreeItem* GetTitleTreeFromAreaContent(const AreaContent* area_content);
extern void DeleteTitleTree(TreeItem* root);
//extern TitleAreaContent* GetTitleContentNode(TitleAreaContent* node, TitleAreaContent* parent = nullptr);
//123