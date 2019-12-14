#include <deque>
#include <Directory.h>
#include <Entry.h>
#include <FindDirectory.h>
#include <Path.h>
#include <stdio.h>

using std::deque;

int
main(void)
{
	// Look up the home folder -- this is much preferable to a
	// hard-coded way of accessing a system path.
	BPath path;
	find_directory(B_USER_DIRECTORY, &path);
	BDirectory dir(path.Path());

	deque<entry_ref> refDeque;

	entry_ref ref;
	while(dir.GetNextRef(&ref) == B_OK)
		refDeque.push_back(ref);

	printf("Contents of the home folder: %s\n", path.Path());
	for (deque<entry_ref>::iterator i = refDeque.begin();
			i != refDeque.end(); i++)
	{
		// Note that because an iterator can be treated like
		// a pointer, we can access each entry_ref's name
		// using the iterator.
		printf("\t%s\n", i->name);
	}
}
