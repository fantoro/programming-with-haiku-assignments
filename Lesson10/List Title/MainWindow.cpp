#include "MainWindow.h"

#include <Application.h>
#include <ListItem.h>
#include <ScrollView.h>
#include <Button.h>

enum
{
	M_RESET_WINDOW = 'rswn',
	M_SET_TITLE = 'sttl'
};

MainWindow::MainWindow(void)
	:	BWindow(BRect(100,100,500,400),"This is just a title",B_TITLED_WINDOW,
				B_ASYNCHRONOUS_CONTROLS | B_QUIT_ON_WINDOW_CLOSE)
{
	BRect r(Bounds());
	BView *top = new BView(r, "topview", B_FOLLOW_ALL,B_WILL_DRAW);
	AddChild(top);
	
	top->SetViewColor(ui_color(B_PANEL_BACKGROUND_COLOR));
	
	BButton *reset = new BButton(BRect(), "resetbutton", "Reset",
			new BMessage(M_RESET_WINDOW),
			B_FOLLOW_RIGHT|B_FOLLOW_BOTTOM);
	
	top->AddChild(reset);
	reset->ResizeToPreferred();
	
	reset->MoveTo(Bounds().right - reset->Bounds().Width() - 10.0,
			Bounds().bottom - reset->Bounds().Height() - 10.0);
	
	r.InsetBy(10.0,10.0);
	
	r.right -= B_V_SCROLL_BAR_WIDTH;

	r.bottom = reset->Frame().top - 10.0 - B_H_SCROLL_BAR_HEIGHT;
	
	fListView = new BListView(r, "colorlist", B_SINGLE_SELECTION_LIST, B_FOLLOW_ALL);
	
	BScrollView *scrollView = new BScrollView("scrollview", fListView,
			B_FOLLOW_ALL,0,true,true);
	top->AddChild(scrollView);
	
	fListView->SetSelectionMessage(new BMessage(M_SET_TITLE));
	
	fListView->AddItem(new BStringItem("This is probably a piece of text"));
	fListView->AddItem(new BStringItem("I think this is a piece of text as well"));
	fListView->AddItem(new BStringItem("Wow this one is a piece of text too!"));
	fListView->AddItem(new BStringItem("Ok I ran out of ideas"));
}


void
MainWindow::MessageReceived(BMessage *msg)
{
	switch (msg->what)
	{
		case M_RESET_WINDOW:
		{
			fListView->DeselectAll();
			break;
		}
		case M_SET_TITLE:
		{
			int32 selection = fListView->CurrentSelection();
			
			if(selection < 0)
			{
				SetTitle("This is just a title");
			}
			
			BStringItem *it = dynamic_cast<BStringItem*>(
					fListView->ItemAt(selection));
			
			if(it)
				SetTitle(it->Text());
			break;
		}
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
