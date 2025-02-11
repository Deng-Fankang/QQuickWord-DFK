#pragma once
#include<string>
#include<vector>
#include<memory>
#include "global.h"
#include "worddefine.h"
#include "qquickword.h"

using namespace std;

extern QQuickWord* main_window;
void ShowMainWindow();
class TitleAreaContent;

struct ImportFileData
{
	ImportFileData(QString file_name, TitleAreaContent* root) {
		this->file_name = file_name;
		this->root = root;
	}

	ImportFileData(const ImportFileData& data) {
		file_name = data.file_name;
		root = data.root;
	}

	QString file_name;
	TitleAreaContent* root;
};

class ImportFilesMgr: public QObject
{
	Q_OBJECT
private:
	ImportFilesMgr();
public:
	static ImportFilesMgr& Instance();
	const vector<ImportFileData>& GetFilesData() const;
	void ImportFiles(QVector<QString> files);
	const int& ImportFilesMgr::GetA() const;
	~ImportFilesMgr() {};

private:
	vector<ImportFileData> m_files_data;

Q_SIGNALS:
	void importFilesChange();
public:
	int a_;
};


