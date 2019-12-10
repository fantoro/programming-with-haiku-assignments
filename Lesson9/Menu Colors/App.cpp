#include "App.h"
#include "MainWindow.h"

App::App(void)
	:	BApplication("application/x-vnd.fantoro-MenuColors")
{
	MainWindow *w = new MainWindow();
	
	w->Show();
}

int
main(void)
{
	App *app = new App();
	app->Run();
	
	delete app;
	
	return 0;
}
