#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <Window.h>
#include <ListView.h>

class MainWindow : public BWindow
{
public:
						MainWindow(void);
			void		MessageReceived(BMessage *msg);
			bool		QuitRequested(void);
			
private:
			BListView	*fListView;
};

#endif
