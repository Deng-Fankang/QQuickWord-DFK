#pragma once
#include "wordimport.h"
#include "wordexport.h"
#include "wordedit.h"


class ModuleManager
{

private:
	ModuleManager();

public:
	static ModuleManager& Instance();
	static void Start();
	static void Stop();

	static WordImport::Module g_word_import;
	static WordEdit::Module g_word_edit;
	static WordExport::Module g_word_export;

	~ModuleManager();
};

