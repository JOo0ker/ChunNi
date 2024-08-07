#include "ToDo/CNToDoPage.h"

#include <QBoxLayout>

#include "ElaScrollPageArea.h"
#include "ToDo/ToDoItemElaCheckBox.h"


namespace CN
{
	CNToDoPage::CNToDoPage(QWidget* parent)
		: ElaScrollPage(parent)
	{
		MainArea = new ElaScrollPageArea(this);
		auto* layout = new QHBoxLayout(MainArea);

		auto* centralWidget = new QWidget(this);
		centralWidget->setWindowTitle("ToDo");
		auto* centerLayout = new QVBoxLayout(centralWidget);

		centerLayout->addWidget(MainArea);
		centerLayout->addStretch();
		centerLayout->setContentsMargins(0, 0, 0, 0);
		addCentralWidget(centralWidget, true, true, 0);

		Init();
	}

	CNToDoPage::~CNToDoPage()
	{
		Save();
	}

	ToDoItemElaCheckBox* CNToDoPage::CreateOneTask(QJsonObject obj)
	{
		const auto newTask = new ToDoItemElaCheckBox();
		newTask->Load(obj);
		AllTask.append(newTask);
		connect(newTask, &ToDoItemElaCheckBox::stateChanged, this, &CNToDoPage::Save);

		MainArea->layout()->addWidget(newTask);
		return newTask;
	}

	void CNToDoPage::Save() const
	{
		QFile file("ToDoData.json");

		if (!file.open(QIODevice::WriteOnly | QIODevice::Text))
		{
			qWarning("Couldn't open ToDoPage write file.");
			return;
		}

		QJsonObject obj;
		QJsonArray arr;
		for(const auto task : AllTask)
		{
			if(auto taskSave = task->Save(); !taskSave.isEmpty())
			{
				arr.append(taskSave);
			}
		}
		obj[TODO_PAGE_STR(Items)] = arr;

		const QJsonDocument newJsonDoc(obj);
		file.write(newJsonDoc.toJson());
		file.close();
	}

	void CNToDoPage::Init()
	{
		QFile file("ToDoData.json");

		// 打开文件
		if (!file.open(QIODevice::ReadWrite | QIODevice::Text))
		{
			qWarning("Couldn't open save file.");
			return;
		}

		// 读取文件内容
		const QByteArray fileData = file.readAll();
		file.close();

		// 解析JSON文档
		const QJsonDocument jsonDoc = QJsonDocument::fromJson(fileData);

		if (jsonDoc.isNull() || !jsonDoc.isObject())
		{
			qWarning("Invalid JSON document.");
			return;
		}

		if(QJsonObject obj = jsonDoc.object(); 
			obj.contains(TODO_PAGE_STR(Items)))
		{
			auto itemsArray = obj[TODO_PAGE_STR(Items)].toArray();
			for(auto item : itemsArray)
			{
				(void)CreateOneTask(item.toObject());
			}
		}
	}
}
