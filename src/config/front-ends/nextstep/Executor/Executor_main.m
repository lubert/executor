/* Generated by the NeXT Project Builder 
   NOTE: Do NOT change this file -- Project Builder maintains it.
*/

#include "rsys/common.h"
#ifndef OPENSTEP
#import <appkit/appkit.h>
#else /* OPENSTEP */

#import <AppKit/AppKit.h>
#endif /* OPENSTEP */

#include "MacAppClass.h"
#include <sys/time.h>
#include <sys/resource.h>

#include "rsys/memory_layout.h"


#if defined(i386)
#define NEXT_STACK_SIZE (512L * 1024)
#endif /* defined(i386) */

#ifndef OPENSTEP
void main(int argc, char *argv[]) {
#else /* OPENSTEP */
int ExecutorArgc;
char **ExecutorArgv;

int main(int argc, char *argv[]) {
#endif /* OPENSTEP */

#if 1	/* THIS IS A TEMPORARY HACK */

  void *where;
  kern_return_t r;
#if defined(i386)
  struct rlimit rl;

  getrlimit(RLIMIT_STACK, &rl);
  rl.rlim_cur = NEXT_STACK_SIZE;
  setrlimit(RLIMIT_STACK, &rl);
#endif /* defined(i386) */

#if !defined (STRICT_OPENSTEP)
#define LENGTH	(TEXT_SEGMENT_START - PAGE_ZERO_START)

  if ((r=vm_deallocate(task_self(), 0, LENGTH))!=KERN_SUCCESS) {
    fprintf(stderr, "Could not deallocate err=%d\n", r);
    exit(-10);
  }

  where = PAGE_ZERO_START;
  if ((r=vm_allocate(task_self(), (vm_address_t *)&where, (vm_size_t) LENGTH,
						     FALSE)) != KERN_SUCCESS) {
    fprintf(stderr, "Could not allocate err=%d\n", r);
    exit(-11);
  }
#endif

#endif

#ifndef OPENSTEP
    [MacAppClass new];
    if ([NXApp loadNibSection:"Executor.nib" owner:NXApp withNames:NO])
	    [NXApp run];
    else
	fprintf(stderr, "death1\n");

	    
    [NXApp free];
    exit(0);
#else /* OPENSTEP */
    ExecutorArgc = argc;
    ExecutorArgv = argv;
    return NSApplicationMain (argc, argv);
#endif /* OPENSTEP */
}
