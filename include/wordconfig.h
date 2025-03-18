#pragma once
#include "qpair.h"
#include "worddefine.h"

namespace WordConfig
{
	void GetTextAreaFormat(TextFormat& text_format);
	void GetTableAreaFormat(TableFormat& table_format, TableAreaContent::CellFormat& cell_format, TextFormat& text_format);
	void GetListAreaFormat(ListTemplateFormat& list_template_format, TextFormat& text_format);
	void GetImageAreaFormat(ImageFormat& image_format);
	void GetTitleAreaFormat(ListTemplateFormat& list_template_format, TextFormat& title_format, int level);

	bool InitWordConfig();
}