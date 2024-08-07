#include "ChunNi.h"

#include <QVBoxLayout>

#include "ElaContentDialog.h"
#include "ElaDockWidget.h"
#include "ElaEventBus.h"
#include "ElaGraphicsItem.h"
#include "ElaGraphicsScene.h"
#include "ElaGraphicsView.h"
#include "ElaLog.h"
#include "ElaMenu.h"
#include "ElaMenuBar.h"
#include "ElaStatusBar.h"
#include "ElaText.h"
#include "ElaTheme.h"
#include "ElaToolBar.h"
#include "ElaToolButton.h"
#include "ElaWidget.h"
#include "ExamplePage/T_BaseComponents.h"
#include "ExamplePage/T_Home.h"
#include "ExamplePage/T_LogWidget.h"
#include "ExamplePage/T_UpdateWidget.h"

#include "ToDo/CNToDoPage.h"


ChunNi::ChunNi(QWidget* parent)
    : ElaWindow(parent)
{
    InitWindow();

    // 额外布局
    InitEdgeLayout();

    // 中心窗口
    InitContent();

    // 拦截默认关闭事件
    this->setIsDefaultClosed(false);
    connect(this, &ChunNi::closeButtonClicked, this, &ChunNi::OnCloseButtonClicked);

    //移动到中心
    moveToCenter();

    TestFunc();
}

ChunNi::~ChunNi()
{
    
}

void ChunNi::OnCloseButtonClicked()
{
	const auto dialog = new ElaContentDialog(this);
    connect(dialog, &ElaContentDialog::rightButtonClicked, this, &ChunNi::closeWindow);
    connect(dialog, &ElaContentDialog::middleButtonClicked, this, &ChunNi::showMinimized);
    dialog->show();
}

void ChunNi::InitWindow()
{
    // Base properties
    //------------------------------
    resize(1240, 740);
    ElaLog::getInstance()->initMessageLog(true);
    eTheme->setThemeMode(ElaThemeType::Dark);
    // setIsNavigationBarEnable(false);
    // setNavigationBarDisplayMode(ElaNavigationType::Compact);
    // setWindowButtonFlag(ElaAppBarType::MinimizeButtonHint, false);

    // User info card
    //------------------------------
    setUserInfoCardPixmap(QPixmap(":/Resource/Image/PigPigIcon.jpg"));
    setUserInfoCardTitle("ChunNi");
    setUserInfoCardSubTitle("624459425@qq.com");

    // Window config
    //------------------------------
    setWindowIcon(QIcon(":/Resource/Image/PigPigIcon.jpg"));
    setWindowTitle("ChunNi");
    // setIsStayTop(true);
    // setUserInfoCardVisible(false);
}

void ChunNi::InitEdgeLayout()
{
    // Menu bar
    //------------------------------
    {
        const auto menuBar = new ElaMenuBar(this);
        menuBar->setFixedHeight(30);
        const auto menuWidget = new QWidget(this);
        const auto menuLayout = new QVBoxLayout(menuWidget);
        menuLayout->setContentsMargins(0, 0, 0, 0);
        menuLayout->addWidget(menuBar);
        menuLayout->addStretch();
        //this->setMenuBar(menuBar);
        this->setCustomWidget(menuWidget);
        this->setCustomWidgetMaximumWidth(500);

        menuBar->addElaIconAction(ElaIconType::Toolbox, tr("Tool"));

        ElaMenu* iconMenu = menuBar->addMenu(ElaIconType::GamepadModern, tr("Model"));
        iconMenu->setMenuItemHeight(27);
        iconMenu->addElaIconAction(ElaIconType::FolderCheck, tr("Model1"), QKeySequence::Save);
        iconMenu->addElaIconAction(ElaIconType::HeartCircleCheck, tr("Model2"));
        iconMenu->addElaIconAction(ElaIconType::CommentCheck, tr("Model3"));
        iconMenu->addSeparator();
        iconMenu->addElaIconAction(ElaIconType::CloudCheck, tr("Model4"));
        iconMenu->addElaIconAction(ElaIconType::CircleCheck, tr("Model5"));

        menuBar->addSeparator();
    }


    // Tool bar
    //------------------------------
    const auto toolBar = new ElaToolBar(tr("Tool Bar"), this);
    toolBar->setAllowedAreas(Qt::TopToolBarArea | Qt::BottomToolBarArea);
    toolBar->setToolBarSpacing(3);
    toolBar->setToolButtonStyle(Qt::ToolButtonIconOnly);
    toolBar->setIconSize(QSize(25, 25));
    // toolBar->setFloatable(false);
    // toolBar->setMovable(false);

    // Get cursor position tool button
    auto* getCursorPositionToolButton = new ElaToolButton(this);
    getCursorPositionToolButton->setElaIcon(ElaIconType::Trowel);
    getCursorPositionToolButton->setText(tr("GetCursorPosition"));
    getCursorPositionToolButton->setToolTip(tr("Get next cursor press position."));
    toolBar->addWidget(getCursorPositionToolButton);

    this->addToolBar(Qt::TopToolBarArea, toolBar);


    // Dock widget
    //------------------------------
    auto* logDockWidget = new ElaDockWidget(tr("Log Msg"), this);
    logDockWidget->setWidget(new T_LogWidget(this));
    this->addDockWidget(Qt::RightDockWidgetArea, logDockWidget);
    resizeDocks({ logDockWidget }, { 200 }, Qt::Horizontal);

    auto updateDockWidget = new ElaDockWidget(tr("To Do"), this);
    updateDockWidget->setWidget(new T_UpdateWidget(this));
    this->addDockWidget(Qt::RightDockWidgetArea, updateDockWidget);
    resizeDocks({ updateDockWidget }, { 200 }, Qt::Horizontal);

    // Status bar
    //------------------------------
    const auto statusBar = new ElaStatusBar(this);
    statusBar->showMessage(tr("Initial successful"));
    this->setStatusBar(statusBar);
}

void ChunNi::InitContent()
{
    _homePage = new T_Home(this);
    _components = new T_BaseComponents(this);

    this->addPageNode("Home", _homePage, ElaIconType::House);
    this->addPageNode("ElaBaseComponents", _components, ElaIconType::CabinetFiling);

    ToDoPage = new CN::CNToDoPage(this);
    this->addPageNode("ToDo", ToDoPage, ElaIconType::Check);

    addFooterNode("About", nullptr, _aboutKey, 0, ElaIconType::User);
    const auto widget = new ElaWidget();
    widget->setWindowModality(Qt::ApplicationModal);
    widget->hide();
    connect(this, &ElaWindow::navigationNodeClicked, this, 
        [=](ElaNavigationType::NavigationNodeType nodeType, const QString& nodeKey) 
        {
            if (_aboutKey == nodeKey)
            {
                widget->show();
            }
        });
    addFooterNode("Setting", new QWidget(this), _settingKey, 0, ElaIconType::GearComplex);
    connect(this, &ChunNi::userInfoCardClicked, this, [=]() { this->navigation(_homePage->property("ElaPageKey").toString()); });
}

void ChunNi::TestFunc()
{

}
