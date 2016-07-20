#ifndef INDEX_H
#define INDEX_H
#include <linux/limits.h>
struct word
{
	char * string;
	struct file * entry;
};
struct file
{
	char filename[PATH_MAX];
	int frequency;
	struct file * next;
};
#endif
