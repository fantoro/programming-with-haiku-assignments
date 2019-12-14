#include "MainWindow.h"

#include <Application.h>
#include <View.h>
#include "PictureView.h"

MainWindow::MainWindow(void)
	:	BWindow(BRect(100,100,500,400),"Main Window",B_TITLED_WINDOW, B_ASYNCHRONOUS_CONTROLS)
{
	SetPulseRate(1000000);
	
	BView *top = new BView(Bounds(), "backgroundcolor", B_FOLLOW_ALL,
				B_WILL_DRAW);
	
	AddChild(top);
	
	top->SetViewColor(ui_color(B_PANEL_BACKGROUND_COLOR));
	
	PictureView *pv = new PictureView();
	
	pv->MoveTo(Bounds().Width()/2.0-pv->Bounds().Width()/2.0,
			Bounds().Height()/2.0-pv->Bounds().Width()/2.0);
	
	top->AddChild(pv);
}


void
MainWindow::MessageReceived(BMessage *msg)
{
	switch (msg->what)
	{
		default:
		{
			BWindow::MessageReceived(msg);
			break;
		}
	}
}


bool
MainWindow::QuitRequested(void)
{
	be_app->PostMessage(B_QUIT_REQUESTED);
	return true;
}
