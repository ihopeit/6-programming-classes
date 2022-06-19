#include<stdio.h>
#include<stdlib.h>

struct astack { 
	int top; 
	unsigned size; 
	int* array; 
}; 

struct queue{ 
    int front, rear, size; 
    unsigned actualSize; 
    int* arr; 
}; 

void enqueue(struct queue * que, int item); // 在队列尾部插入元素
int  dequeue(struct queue * que);  // 从队列头部删除一个元素
int  front(struct queue * que);    // 获取队列的第一个元素（头部）
int  rear(struct queue * que);     // 获取队列的最后一个元素（尾部）

// linked list 
struct node{
    int data;
    struct node *next;
};

struct node * insert_end(struct node *p, int element);
struct node * insert_begin(struct node *p, int element);
void delete_begin(struct node *p);
void delete_end(struct node *p);
void l_delete(struct node *p, int element);
void display(struct node *p); // 显示所有元素

int main()
{ 
  int val,n;
  char c;
  struct node *p;
  p = NULL;

  do{
     printf("\n************************* MENU ************************");
	 printf("\n1.INSERT AT END");
	 printf("\n2.INSERT AT BEG");
	 printf("\n3.DELETE A PARTICULAR ELE");
	 printf("\n4.DELETE FROM BEG");
	 printf("\n5.DELETE FROM END");
	 printf("\n6.DISPLAY");
	 printf("\n7.EXIT");
	 printf("\nenter ur choice: ");
	 scanf("%d",&n);
	 switch(n){
        case 1: printf("\nenter the value: ");
			 scanf("%d",&val);
			 p = insert_end(p, val);
			 break;
		 case 2: printf("\nenter the value: ");
			 scanf("%d",&val);
			 p = insert_begin(p, val);
			 break;
		 case 3: printf("\nenter the value: ");
			 scanf("%d",&val);
			 l_delete(p, val);
			 break;
		 case 4: 
			 delete_begin(p);
			 break;
		 case 5: 
			 delete_end(p);
			 break;
		 case 6: 
             display(p);
		 	 break;
		 case 7: 
            exit(0);
		 	break;
		 default: 
            printf("\n Wrong Choice!");
		 	break;
		}
	 printf("\n do u want to cont(y/n)... ");
     fflush(stdin); 
     c = getchar();
	}while('y' == c || 'Y' == c || '\n' == c);

 }

 struct node * insert_end(struct node *p1, int ele)
 {
	struct node *tmp = p1; // 指向节点的指针
	struct node *tmp1 = (struct node*)malloc(sizeof(struct node));
	tmp1->data = ele; // 为创建的新节点 tmp1 设置 data 属性的值
	tmp1->next = NULL;
	if(p1 == NULL)
        p1 = tmp1;
	else { // 遍历到最后的叶子节点, tmp 指针移动到最后一个子节点
        while(tmp->next != NULL)
            tmp = tmp->next;
        tmp->next = tmp1; // 在叶子节点上追加新的节点
	}
    return p1;
 }

struct node * insert_begin(struct node *p, int ele)
{
	 struct node *tmp = p;
	 struct node *tmp1=(struct node*)malloc(sizeof(struct node));
	 tmp1->data=ele;
	 tmp1->next=p;
	 p=tmp1;
     return p;
}

void l_delete(struct node *p, int ele)
{
     struct node *tmp = p;
	 struct node *pre = tmp;
	 while(tmp!=NULL)
		{if(tmp->data==ele)
		    { if(tmp==p)
		       {p=tmp->next;
			free(tmp);
			return;
			}
		      else
			{pre->next=tmp->next;
			 free(tmp);
			 return;
			 }
		    }
		 else
		    { pre=tmp;
		      tmp=tmp->next;
		    }
		}
	  printf("\n no match found!! ");
 }
 
void delete_begin(struct node *p)
{	
	struct node *tmp = p;
	if(p==NULL)
		printf("\n no element to be deleted!! ");
	else
	{
		printf("\nelement deleted - %d", p->data);
		p=p->next;
	}

 }
 
void delete_end(struct node *p)
{	
	struct node *tmp = p;
	struct node* pre;
	if(p==NULL)
		printf("\n no element to be deleted!! ");
	else if(p->next==NULL)
	{
		printf("\nelement deleted - %d", p->data);
		p=NULL;

	}
		
	else
	{
		while(tmp->next!=NULL){
			pre=tmp;
			tmp=tmp->next;
		}
		pre->next=NULL;
		printf("\nelement deleted - %d", tmp->data);
		
	}

 }

void display(struct node *p)
{
    printf("display elements in p\n");
	struct node *tmp = p;
 	while(tmp!=NULL){
        printf("\n %d",tmp->data);
	 	tmp=tmp->next;
	}
}