#include "MainWindow.h"

#include <MenuBar.h>
#include <Menu.h>
#include <MenuItem.h>
#include <View.h>

enum
{
	M_SET_COLOR_RED = 'sred',
	M_SET_COLOR_PINK = 'spnk',
	M_SET_COLOR_GREEN = 'sgrn',
	M_SET_COLOR_BLUE = 'sblu',
	M_SET_COLOR_LBLUE = 'slbl',
	M_SET_COLOR_BLACK = 'sblk'
};

MainWindow::MainWindow(void)
	:	BWindow(BRect(100,100,500,400),"MenuColors",B_TITLED_WINDOW,
				B_ASYNCHRONOUS_CONTROLS | B_QUIT_ON_WINDOW_CLOSE)
{
	BView *box = new BView(BRect(100,100,300,200),"colorbox", B_FOLLOW_ALL, B_WILL_DRAW);
	AddChild(box);
	box->SetViewColor(0,0,160);
	
	BRect r(Bounds());
	r.bottom = 10;
	
	BMenuBar *mBar = new BMenuBar(r,"menubar");
	AddChild(mBar);
	
	BMenu *m = new BMenu("Colors");
	m->AddItem(new BMenuItem("Red", new BMessage(M_SET_COLOR_RED),'R'));
	m->AddItem(new BMenuItem("Pink", new BMessage(M_SET_COLOR_PINK),'P'));
	m->AddItem(new BMenuItem("Green", new BMessage(M_SET_COLOR_GREEN),'G'));
	m->AddItem(new BMenuItem("Blue", new BMessage(M_SET_COLOR_BLUE),'B'));
	m->AddItem(new BMenuItem("Light Blue", new BMessage(M_SET_COLOR_LBLUE),'V'));
	m->AddItem(new BMenuItem("Black", new BMessage(M_SET_COLOR_BLACK),'K'));
	m->AddItem(new BMenuItem("Quit", new BMessage(B_QUIT_REQUESTED),'Q'));
	
	mBar->AddItem(m);
}

void
MainWindow::MessageReceived(BMessage *m)
{
	BView *box = FindView("colorbox");
	switch(m->what)
	{
		case M_SET_COLOR_RED:
		{
			box->SetViewColor(255,0,0);
			break;
		}
		case M_SET_COLOR_PINK:
		{
			box->SetViewColor(255,200,200);
			break;
		}
		case M_SET_COLOR_GREEN:
		{
			box->SetViewColor(0,255,0);
			break;
		}
		case M_SET_COLOR_BLUE:
		{
			box->SetViewColor(0,0,160);
			break;
		}
		case M_SET_COLOR_LBLUE:
		{
			box->SetViewColor(64,64,255);
			break;
		}
		case M_SET_COLOR_BLACK:
		{
			box->SetViewColor(0,0,0);
			break;
		}
		default:
		{
			BWindow::MessageReceived(m);
			break;
		}
	}
	box->Invalidate();
}	
