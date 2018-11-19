/***********************************************************************************
** MIT License                                                                    **
**                                                                                **
** Copyright (c) 2018 Victor DENIS (victordenis01@gmail.com)                      **
**                                                                                **
** Permission is hereby granted, free of charge, to any person obtaining a copy   **
** of this software and associated documentation files (the "Software"), to deal  **
** in the Software without restriction, including without limitation the rights   **
** to use, copy, modify, merge, publish, distribute, sublicense, and/or sell      **
** copies of the Software, and to permit persons to whom the Software is          **
** furnished to do so, subject to the following conditions:                       **
**                                                                                **
** The above copyright notice and this permission notice shall be included in all **
** copies or substantial portions of the Software.                                **
**                                                                                **
** THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR     **
** IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,       **
** FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE    **
** AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER         **
** LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,  **
** OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE  **
** SOFTWARE.                                                                      **
***********************************************************************************/

#include "DemoPlugin.hpp"

#include <QVBoxLayout>
#include <QDialog>
#include <QPushButton>
#include <QLabel>

#include <QMenu>
#include <QMessageBox>

#include "DemoPluginSideBar.hpp"

#include "Web/WebView.hpp"
#include "Web/WebHitTestResult.hpp"

#include "Plugins/PluginProxy.hpp"
#include "Utils/SideBarManager.hpp"

#include "Application.hpp"

namespace Sn
{
DemoPlugin::DemoPlugin()
{
	// Empty
}

PluginProp DemoPlugin::pluginProp()
{
	PluginProp props{};

	props.name = "Demo Plugin";
	props.info = "Demo of minimal plugin";
	props.desc = "Very simple minimal plugin example";
	props.version = "1.0.0";
	props.author = "Victor DENIS <victordenis01@gmail.com";
	props.hasSettings = true;

	return props;
}

void DemoPlugin::init(InitState state, const QString& settingsPath)
{
	m_settingsPath = settingsPath;
	m_sideBar = new DemoPluginSideBar(this);

	Application::instance()->plugins()->registerAppEventHandler(PluginProxy::MousePressHandler, this);
	Application::instance()->addSidebar("demoplugin-sidebar", m_sideBar);
}

void DemoPlugin::unload()
{
	Application::instance()->removeSidebar(m_sideBar);

	delete m_sideBar;
	delete m_settings.data();
}

bool DemoPlugin::testPlugin()
{
	return true;
}

void DemoPlugin::showSettings(QWidget* parent)
{
	if (!m_settings) {
		m_settings = new QDialog(parent);

		QPushButton* button{new QPushButton("Demo Plugin v1.0.0")};
		QPushButton* closeButton{new QPushButton(tr("Close"))};
		QLabel* label{new QLabel()};

		label->setPixmap(QPixmap(":images/aboutsielo.png"));

		QVBoxLayout* layout = {new QVBoxLayout(m_settings.data())};
		layout->addWidget(label);
		layout->addWidget(button);
		layout->addWidget(closeButton);

		m_settings.data()->setAttribute(Qt::WA_DeleteOnClose);
		m_settings.data()->setWindowTitle(tr("Demo Plugin Settings"));
		m_settings.data()->setWindowIcon(QIcon(":images/ic_sielo.png"));

		connect(closeButton, &QPushButton::clicked, m_settings.data(), &QDialog::close);
	}

	m_settings.data()->show();
	m_settings.data()->raise();
}

void DemoPlugin::populateWebViewMenu(QMenu* menu, WebView* view, const WebHitTestResult& result)
{
	m_view = view;

	QString title{};

	if (!result.imageUrl().isEmpty())
		title += " on image";

	if (!result.linkUrl().isEmpty())
		title += " on link";

	if (result.isContentEditable())
		title += " on input";

	menu->addAction(tr("My first plugin action") + title, this, &DemoPlugin::actionSlot);
}

bool DemoPlugin::mousePress(const Application::ObjectName& objName, QObject* obj, QMouseEvent* event)
{
	qDebug() << "DemoPlugin: mousePress" << objName << obj << event;

	return false;
}

void DemoPlugin::actionSlot()
{
	QMessageBox::information(m_view, tr("Hello"), tr("First plugin action works!"));
}
}
