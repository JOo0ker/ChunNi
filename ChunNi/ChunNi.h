#pragma once


#include "ElaWindow.h"

namespace CN
{
	class CNToDoPage;
}

class T_Home;
class T_BaseComponents;

class ChunNi : public ElaWindow
{
    Q_OBJECT

public:
    ChunNi(QWidget *parent = nullptr);
    ~ChunNi();

    Q_SLOT void OnCloseButtonClicked();

    void InitWindow();
    void InitEdgeLayout();
    void InitContent();

private:
    void TestFunc();

    T_Home* _homePage{ nullptr };
    T_BaseComponents* _components{ nullptr };

    CN::CNToDoPage* ToDoPage{ nullptr };

    QString _aboutKey{ "" };
    QString _settingKey{ "" };
};
