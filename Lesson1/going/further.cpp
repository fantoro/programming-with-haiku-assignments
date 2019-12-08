// Include <list> instead of <deque>
#include <list>

#include <Directory.h>
#include <Entry.h>
#include <FindDirectory.h>
#include <Path.h>
#include <stdio.h>

// Replace std::deque with std::list.
using std::list;

int
main(void)
{
	// Look up the home folder -- this is much preferable to a
	// hard-coded way of accessing a system path.
	BPath path;
	find_directory(B_USER_DIRECTORY, &path);
	BDirectory dir(path.Path());
	
	// Use a list instead of a deque and use refList everywhere instead of refDeque
	list<entry_ref> refList;

	entry_ref ref;
	while(dir.GetNextRef(&ref) == B_OK)
		refList.push_back(ref);

	printf("Contents of the home folder: %s\n", path.Path());
	for (list<entry_ref>::iterator i = refList.begin();
			i != refList.end(); i++)
	{
		// Note that because an iterator can be treated like
		// a pointer, we can access each entry_ref's name
		// using the iterator.
		printf("\t%s\n", i->name);
	}
}
