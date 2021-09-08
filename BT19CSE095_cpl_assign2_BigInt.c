#include<stdio.h>
#include<stdlib.h>
#include<string.h>

//the basic idea is to take big integers as strings, store them in linked lists and operate on them.
typedef struct node_tag
{
	int data;
	struct node_tag * next;
}bigint;

typedef struct dl_node_tag
{
	int data;
	struct dl_node_tag * prev;
	struct dl_node_tag * next;
}bigint_dl;

bigint * MakeNode(int n)
{
	bigint * lptr;
	lptr=(bigint *)malloc(sizeof(bigint));
	lptr->data=n;
	lptr->next=NULL;
	return lptr;
}

bigint_dl * Make_dl(int n)
{
	bigint_dl * lptr;
	lptr=(bigint_dl *)malloc(sizeof(bigint_dl));
	lptr->data=n;
	lptr->next=NULL;
	lptr->prev=NULL;
	return lptr;
}

//the digits get stored in linked lists from LSB to MSB.

bigint * MakeInt(char a[])
{
	int i,len=strlen(a),n;
	bigint * nptr, * head=NULL;
	
	for(i=0;i<len;i++)
	{
		if(a[i]!='-')
		{
			n=a[i]-'0';
			nptr=MakeNode(n);
			nptr->next=head;
			head=nptr;
		}
			
	}
	return head;	
}

void printList(bigint * head)
{
	while(head!=NULL)
	{
		printf("%d",head->data);
		head=head->next;
	}
}

void printDl(bigint_dl * head)
{
	while(head!=NULL)
	{
		printf("%d",head->data);
		head=head->next;
	}
}

//this function reverses the linked list.
bigint * reverse(bigint * head)
{
	bigint * mid, * trail;
	mid=NULL;
	while(head!=NULL)
	{
		trail=mid;
		mid=head;
		head=head->next;
		mid->next=trail;
	}
	return mid;
}

//another linked list is created to store the result of every addition. The carry digit is dealt with accordingly
//The pointer to linked list representing the sum is returned.
bigint * addition(bigint * int1, bigint * int2)
{
	int n,carry=0;
	bigint * head=NULL, * nptr;
	while((int1!=NULL)&&(int2!=NULL))
	{
		n=(int1->data + int2->data +carry)%10;
		nptr=MakeNode(n);
		nptr->next=head;
		head=nptr;
		carry=(int1->data + int2->data +carry)/10;
		int1=int1->next;
		int2=int2->next;
	}
	
//	the remaining digits are being copied.
	while(int1!=NULL)
	{
		n=(int1->data + carry)%10;
		nptr=MakeNode(n);
		nptr->next=head;
		head=nptr;
		carry=(int1->data + carry)/10;	
		int1=int1->next;
	}
	
	while(int2!=NULL)
	{
		n=(int2->data +carry)%10;
		nptr=MakeNode(n);
		nptr->next=head;
		head=nptr;
		carry=(int2->data +carry)/10;
		int2=int2->next;
	}
	
//	if single carry remains in the end of addition.
	if(carry>0)
	{
		n=(carry)%10;
		nptr=MakeNode(n);
		nptr->next=head;
		head=nptr;	
	}
	
	return head;
}

//this is done after analysing the greater and smaller of the two big integers.
bigint * compute_sub(bigint * greater, bigint * smaller)
{
	int digit,n,carry=0;
	bigint * head=NULL, * nptr;
	
	while(smaller!=NULL)                   //subtraction by carry forward method.
	{
		if(greater->data < (smaller->data + carry))
		{
			digit=greater->data+10;
			n=digit-(smaller->data + carry);
			nptr=MakeNode(n);
			nptr->next=head;
			head=nptr;
			carry=1;                         //carry becomes 1 if digit to be subtracted becomes greater.
			smaller=smaller->next;
			greater=greater->next;
		}
		else
		{
			n=greater->data-(smaller->data + carry);
			nptr=MakeNode(n);
	
			nptr->next=head;
			head=nptr;
			carry=0;
			smaller=smaller->next;
			greater=greater->next;
		}
	}
	
	while(greater!=NULL)                 //copying the rest of the digits.
	{
		if(greater->data < (carry))
		{
			digit=greater->data+10;
			n=digit-(carry);
			nptr=MakeNode(n);
			nptr->next=head;
			head=nptr;
			carry=1;
			greater=greater->next;
			
		}
		else
		{
			n=greater->data-(carry);
			nptr=MakeNode(n);
			nptr->next=head;
			head=nptr;
			carry=0;
			greater=greater->next;
			
		}	
	}
	while(head->data==0)                    //clearing the preceeding zeroes.
	{
		nptr=head;
		head=head->next;
		free(nptr);
	}
	return head;
}

// function to determine the greater of the two integers.
bigint * subtraction(char str1[],char str2[],bigint * int1, bigint * int2)
{
	bigint * greater, * smaller, * ret_val,* lptr;
	int len1=strlen(str1),len2=strlen(str2),i,flag=1;
	
	if(len1>len2) 
	{
		greater=int1;
		smaller=int2;
	}	
	else if(len1<len2) 
	{
		greater=int2;
		smaller=int1;	
	}
	else                   //if string lengths are similar, we compare the individual digits.
	{
		i=0;
		while(i!=len1 && str1[i]==str2[i])
		{
			i++;
		}
		if(i!=len1 && str1[i]>str2[i])
		{
			greater=int1;
			smaller=int2;	
		}
		else
		{
			if(i!=len1)
			{
				greater=int2;
				smaller=int1;	
			}
			else
			{
				flag=0;
				lptr=(bigint *)malloc(sizeof(bigint));   //returns 0 if both numbers are equal.
				lptr->data=0;
				lptr->next=NULL;
				ret_val=lptr;
			}        	
		}
	}
	if(flag==1)
	{
		ret_val=compute_sub(greater,smaller);
	}
	if(smaller==int1)
		ret_val->data=(-1)*ret_val->data;
		return ret_val;
		
	
}

//multiplication is done using doubly linked lists. 
bigint_dl * multiply(bigint * int1, bigint * int2)
{
	bigint * ptr1=int1, * ptr2=int2;
	bigint_dl * ptr=NULL, * lptr, * head=NULL, * last=NULL;
	int n,carry=0,carry1=0,count=0,temp;
	
	while(ptr2!=NULL)
	{
		carry=carry1=0;
		ptr=last;                                       // ptr in the pointer to the current place value in result linked 
		ptr1=int1;                                      // list
		
		while(ptr1!=NULL)
		{
			     											//carry is passed on to next place value.
			
			if(ptr!=NULL)                              // if a node exists at that place value in result linked list, 
			{                                           // then add the generated number and previous carry and the value 
				temp=ptr->data+ptr1->data*ptr2->data+carry+carry1; 
				carry=(ptr1->data*ptr2->data+carry)/10;                 // originally present at that place value.
				ptr->data=(temp)%10;
				carry1=(temp)/10;
				ptr=ptr->prev;
			}
			else
			{   
			    temp=(ptr1->data*ptr2->data+carry+carry1)%10;
			    carry=(ptr1->data*ptr2->data+carry)/10;
			    if(ptr2!=int2)
					carry1= (ptr1->data*ptr2->data+carry+carry1)/10;       //if no node exists at current place value, then create one node 
				lptr=Make_dl(temp);                      				  //representing that place value.
				count++;
				if(count==1)
				{
					last=lptr;	
				}	
				lptr->next=head;
				lptr->prev=NULL;
				if(head!=NULL)
				{
					head->prev=lptr;
				}
				head=lptr;
			}
			
			ptr1=ptr1->next;
		}
		temp=(carry+carry1);                           //if the digits of second string are exhausted and the carry still
		if(temp!=0)                                  // persists then a node is created for that place value as well.
			{
				lptr=Make_dl(temp%10);
				temp=temp/10;	
				lptr->next=head;
				lptr->prev=NULL;
				if(head!=NULL)
				{
					head->prev=lptr;
				}
				head=lptr;
			}
			
		ptr2=ptr2->next;
		last=last->prev;
	}
	return head;
}



int main()
{
	bigint * int1, * int2,* add, * sub; 
	bigint_dl * mult, * lptr;
	char str1[5000],str2[5000],lower[5000],higher[5000];
	int choice,i;
	
	printf("enter first integer\n");
	scanf("%s",str1);
	printf("enter second integer\n");
	scanf("%s",str2);
	
	int1=MakeInt(str1);
	int2=MakeInt(str2);
	
	//operations are implemented in loop until the time exit option is selected.
	do{
		printf("\n*************************************************************\n");
		printf("ENTER YOUR CHOICE\n");
		printf(" 1.ADDITION\n 2.SUBTRACTION\n 3.MULTIPLICATION\n 4.EXIT\n");
		printf("**************************************************************\n");
		scanf("%d",&choice);
		switch(choice)
		{
			case 1:
				if(((str1[0]=='-')||(str2[0]=='-'))&&str1[0]!=str2[0]) // if either of the numbers is negative
				{
					if(str1[0]=='-')
					{
						for(i=0;i+1<strlen(str1);i++)
							lower[i]=str1[i+1];
								
						strcpy(higher,str2);
							
						add=subtraction(higher,lower,int2,int1);      // result is difference of absolute values of 
					}                                                 // integers with proper signs.
					else 
					{
						for(i=0;i+1<strlen(str2);i++)
							lower[i]=str2[i+1];
								
						strcpy(higher,str1);
								
						add=subtraction(higher,lower,int1,int2);
					}
				}
				else if((str1[0]=='-')&&(str2[0]=='-'))            //if both numbers are negtive, result is negative
				{                                                  //with magnitude being equal to  sum of absolute values
					add=addition(int1,int2);                      // of the two.
					add->data=add->data*(-1);
				}
				else
				{                                                 // in case both numbers are positive,
					add=addition(int1,int2);	                  // result is basic addition of the two.
				}
				printList(add);
				break;
					
			case 2:
				if(((str1[0]=='-')||(str2[0]=='-'))&&str1[0]!=str2[0]) //if either of the two is negative,
				{                                                      
					if(str1[0]=='-')
					{
						sub=addition(int1,int2);               //if first string is negative then result is negative 
						sub->data=sub->data*(-1);              //sum of absolute values of the two
					}
					else 
					{	                                       // if second is negative, result is sum of values of 
						sub=addition(int1,int2);               //the two.
					}
				}
				else if((str1[0]=='-')&&(str2[0]=='-'))         //if both are negative then first number is passed 
				{                                               //as second parameter to suntraction function.
					for(i=0;i+1<strlen(str1);i++)               // example: -10 - (-7) = -10 + 7 
						lower[i]=str1[i+1];                 // can be treated as 10 being subtracted from 7 
								
					for(i=0;i+1<strlen(str2);i++)
							higher[i]=str2[i+1];
							
					sub=subtraction(higher,lower,int2,int1);	
				}
				else
				{                                               // if both are positive, then result is difference of the 
					sub=subtraction(str1,str2,int1,int2);       // two.
				}
				printList(sub);	
				break;
					
			case 3:
				mult=multiply(int1,int2);
				if(((str1[0]=='-')||(str2[0]=='-'))&&str1[0]!=str2[0]) // if either of the two is negative then result is 
				mult->data=mult->data*(-1);                        // negative.
					
				lptr=mult;
				while((lptr->data==0)&&(mult->next!=NULL))
				{
					mult=mult->next;
					free(lptr);
					lptr=mult;
				}
																											                        
				printDl(mult);
				break;
					
				
					
			case 4:
				printf("EXITED SUCCESSFULLY\n");
				break;
			default:
				printf("INVALID CHOICE");
		}
				
	}while(choice>=1 && choice<4);
		
	return 0;
	
}

