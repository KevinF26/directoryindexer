#include<stdio.h>
#include<stdlib.h>
#include<tok.h>
#include<sorted-list.h>
#include<string.h>
#include<index.h>
#include <unistd.h>
#include <sys/types.h>
#include <dirent.h>
#include<sys/stat.h>
void destruct(void * x)
{
	struct word * n=(struct word *)x;
	free(n);
        return;
}
int cmp(void * x, void * y)
{
	struct word * a=(struct word *)x;
	struct word * b=(struct word *)y;
        return strcmp(a->string,b->string);
}

SortedListPtr indexedwords;

void freqinc(SortedListPtr list,char * token, char * filename)
{
	struct node * curr=list->head;
	struct word * currword;
	if(curr!=NULL)
	{
		currword=(struct word *)curr->data;
	}
	else
	{
		currword=malloc(sizeof(struct word));
		currword->string=token;
		currword->entry=NULL;
		SLInsert(list,currword);
	}
	struct file * currfile;
	struct file * prevfile;
	int tempfreq;
	char tempfilename[PATH_MAX];
	while(strcmp(token,currword->string)>=0)
	{
		//correct word
		if(strcmp(token,currword->string)==0)
		{
			if(currword->entry==NULL)
			{
				struct file * entry=malloc(sizeof(struct file));
				strcpy(entry->filename,filename);
				entry->frequency=1;
				entry->next=NULL;
				currword->entry=entry;
				return;
			}
			if(strcmp((currword->entry)->filename,filename)==0)
			{
				(currword->entry)->frequency++;
				return;
			}
			prevfile=currword->entry;
			currfile=prevfile->next;
			while(currfile!=NULL)
			{
				if(strcmp(currfile->filename,filename)==0)
				{
					currfile->frequency++;
					if((currfile->frequency==prevfile->frequency && strcmp(currfile->filename,prevfile->filename)) || currfile->frequency>prevfile->frequency)
					{
						strcpy(tempfilename,currfile->filename);
						tempfreq=currfile->frequency;
						strcpy(currfile->filename,prevfile->filename);
						currfile->frequency=prevfile->frequency;
						strcpy(prevfile->filename,tempfilename);
						prevfile->frequency=tempfreq;
					}
					return;
				}
				prevfile=currfile;
				currfile=currfile->next;
			}
			struct file * entry=malloc(sizeof(struct file));
                        strcpy(entry->filename,filename);
                        entry->frequency=1;
                        entry->next=NULL;
			prevfile->next=entry;
			return;
		}
		curr=curr->next;
		if(curr!=NULL)
		{
			currword=(struct word *)curr->data;
		}
		else
		{
			break;
		}
	}
	struct word * n=malloc(sizeof(struct word));
        n->string=token;
        struct file * entry=malloc(sizeof(struct file));
        strcpy(entry->filename,filename);
        entry->frequency=1;
        n->entry=entry;
        SLInsert(list,n);
}
void indexfile(FILE * read, char * filename)
{
	TokenizerT* input;
	char * ts=malloc(1024);
        char* currentToken;
        while(fgets(ts,1024,read)!=NULL)
        {
                input=TKCreate(ts);
                currentToken=TKGetNextToken(input);
                while(currentToken!=0)
                {
                        freqinc(indexedwords,currentToken,filename);

                        currentToken=TKGetNextToken(input);
                }
                TKDestroy(input);
        }

}
void traversedir(char * name)
{

	DIR * dirptr;
	struct dirent * entry;
	char * entryname;
	char nentry[PATH_MAX];;

	struct stat s;
	if( stat(name,&s) == 0 )
	{
	    if( s.st_mode & S_IFDIR )
	    {
	        dirptr=opendir(name);
		while(entry=readdir(dirptr))
		{
			entryname=entry->d_name;
			if(*entryname!='.')
			{
				sprintf(nentry,"%s/%s",name,entryname);
				traversedir(nentry);
			}
		}
	    }
	    else if( s.st_mode & S_IFREG )
	    {	
		FILE * read=fopen(name,"r");
		indexfile(read,name);
	    }
	}
}

int main(int argc, char **argv) {
  if(argc>1 && argc<4)
  {
	indexedwords=SLCreate(&cmp,&destruct);
	char * first=argv[2];
	traversedir(first);

	FILE * tobewritten=fopen(argv[1],"w");
	
	struct node * curr=indexedwords->head;
	struct word * currword;
	fprintf(tobewritten,"{\"list\" : [\n");
	while(curr!=NULL)
	{
		currword=(struct word *) curr->data;
		fprintf(tobewritten,"\t{\"%s\" : [\n",currword->string);
		struct file * currfile=currword->entry;
		while(currfile!=NULL)
		{
			if(currfile->next==NULL)
			{
				fprintf(tobewritten,"\t\t{\"%s\" : %d}\n",currfile->filename,currfile->frequency);
			}
			else
			{
				fprintf(tobewritten,"\t\t{\"%s\" : %d},\n",currfile->filename,currfile->frequency);
			}
			currfile=currfile->next;
		}
		if(curr->next!=NULL)
		{
			fprintf(tobewritten,"\t]},\n");
		}
		else fprintf(tobewritten,"\t]}\n");
		curr=curr->next;
	}
	fprintf(tobewritten,"]}");
  }
  return 0;
}

