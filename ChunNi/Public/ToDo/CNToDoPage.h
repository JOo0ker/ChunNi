#pragma once
#include "ElaScrollPage.h"
#include "ToDo/ToDoItemElaCheckBox.h"


class ElaScrollPageArea;

#define TODO_PAGE_STR(N) INDEX_STRING(TODO_PAGE_LIST, ToDoPageKey, N)

namespace CN
{
	struct TimePoint;
	class ToDoItemElaCheckBox;

	enum ToDoPageKey
	{
		Items
	};

	const QStringList TODO_PAGE_LIST
	{
		"items"
	};

	class CNToDoPage : public ElaScrollPage
	{
		Q_OBJECT
	public:
		CNToDoPage(QWidget* parent = nullptr);
		~CNToDoPage() override;

		[[nodiscard]] ToDoItemElaCheckBox* CreateOneTask(QJsonObject obj);

	public Q_SLOTS:
		Q_SLOT void Save() const;

	private:
		void Init();

		ElaScrollPageArea* MainArea{ nullptr };
		QVector<ToDoItemBase*> AllTask;
	};
}

