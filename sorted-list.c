#include<sorted-list.h>

SortedListPtr SLCreate(CompareFuncT cf, DestructFuncT df)
{
	SortedListPtr list=malloc(sizeof(struct SortedList));
	list->cf=cf;
	list->df=df;
	list->head=NULL;
	return list;
}

void SLDestroy(SortedListPtr list)
{
	if(list==NULL)
	{
		return;
	}
	if(list->head==NULL)
	{
		free(list);
		list=NULL;
		return;
	}
	struct node* curr=(list->head)->next;
	struct node* rm=curr;
	while(curr !=NULL)
	{
		curr=curr->next;
		(list->df)(rm->data);
		free(rm);
		rm=NULL;
		rm=curr;
	}
	free(list);
	list=NULL;
}

int SLInsert(SortedListPtr list, void *newObj)
{
	if(list->head==NULL)
        {
               	(list->head)=malloc(sizeof(struct node));
	        (list->head)->data=newObj;
		(list->head)->refnum=1;
                return 0;
        }
   	struct node *curr=list->head;
       	struct node *prev=NULL;
	struct node *n=malloc(sizeof(struct node));
	n->refnum=1;
        n->data=newObj;
        n->next=NULL;
        if((list->cf)(newObj,curr->data)<0)
        {
               	n->next=curr;
               	list->head=n;
               	return 0;
        }
        while(curr!=NULL && ((list->cf)(newObj,curr->data)>0))
        {
                prev=curr;
                curr=curr->next;
        }
        if(curr!=NULL && ((list->cf)(newObj,curr->data)==0))
        {
		(list->df)(n->data);
		free(n);
		n=NULL;
                return 1;
        }
        prev->next=n;
        n->next=curr;
        return 0;
}

int SLRemove(SortedListPtr list, void *newObj)
{
	struct node* temp;
	struct node* curr;
	struct node* prev;
        if(list->head==NULL)
        {
                return 1;
        }
        if((list->cf)(newObj,(list->head)->data)==0)
        {
		temp=list->head;
                list->head=(list->head)->next;
		if(temp->refnum==1)
		{
			(list->df)(temp->data);
			free(temp);
			temp=NULL;
		}
                return 0;
        }
        curr=(list->head)->next;
        prev=list->head;
        while(curr!=NULL)
        {
                if((list->cf)(newObj,curr->data)==0)
                {
                        prev->next=curr->next;
			if(curr->refnum==1)
			{
				(list->df)(curr->data);
				free(curr);
				curr=NULL;
			}
                        return 0;
                }
                prev=curr;
                curr=curr->next;
        }
	return 1;
}

SortedListIteratorPtr SLCreateIterator(SortedListPtr list)
{
	if(list->head!=NULL)
	{
		SortedListIteratorPtr iterator=malloc(sizeof(struct SortedListIterator));
		iterator->item=list->head;
		(list->head)->refnum++;
		return iterator;
	}
	return NULL;
}

void SLDestroyIterator(SortedListIteratorPtr iter)
{
	if(iter->item==NULL)
	{
		return NULL;
	}
	if((iter->item)->refnum==1)
	{
		free(iter->item);
		iter->item=NULL;
	}
	else
	{
		(iter->item)->refnum--;
	}
	free(iter);
	iter=NULL;
}

void * SLGetItem( SortedListIteratorPtr iter )
{
	if(iter->item!=NULL)
	{
		return((iter->item)->data);
	}
	else
	{
		return(NULL);
	}
}

void * SLNextItem(SortedListIteratorPtr iter)
{
	struct node * temp;
	if(iter->item!=NULL)
	{
		if((iter->item)->refnum==1)
		{
			temp=iter->item;
			iter->item=(iter->item)->next;
			free(temp);
			temp=NULL;
		}
		else
		{
			(iter->item)->refnum--;
			iter->item=(iter->item)->next;
		}
		if(iter->item!=NULL)
		{
			return (iter->item)->data;
		}
		else
		{
			return NULL;
		}
	}
	return NULL;
}
	
