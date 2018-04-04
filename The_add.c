#include<stdio.h>
#include<stdlib.h>

typedef struct _node{     //定义节点Node
	double coef;
	int expon;
	struct _node *link;
} Node;

typedef Node* PNode;     //定义指向节点Node的指针类型PNode

void Attach(double c,int e,PNode *pRear);
PNode ReadPoly();
PNode Add(PNode p1, PNode p2);
PNode Mult(PNode p1, PNode p2);
void PrintPoly(PNode p);

int main(void)
{
	PNode P1,P2,Resultadd,Resultmult;
	//读入多项式1
	P1 = ReadPoly();
	//读入多项式2
	P2 = ReadPoly();
	//多项式加法、乘法运算，并返回结果链表的头指针
	Resultadd = Add(P1,P2);
	Resultmult = Mult(P1,P2);
	//输出结果多项式
	PrintPoly(Resultadd);
	PrintPoly(Resultmult);
	
	return 0;
}



PNode ReadPoly(){
	PNode p,rear,t;
	int e,n,i;
	double c;

	p = (PNode)malloc(sizeof(Node));    //生成一个空的头结点
	p->link=NULL;
	rear = p;
	
	scanf("%d",&n);
	for (i=0;i<n;i++){             //将系数和指数读入链表中
		scanf("%lf %d",&c,&e);          
		Attach(c,e,&rear);         //用Attach函数将读入的一组系数、指数插入链表末尾
	}

	t = p;p=p->link;free(t);       //将临时生成的头结点删除
	return p;
}

//申请一个新的节点读入一组coefficient和exponent，并连接到已有链表的尾部。
void Attach(double c,int e,PNode *pRear) 
{
	PNode p;
	//使用malloc函数申请一个新节点
	p = (PNode)malloc(sizeof(Node));
	//对新节点赋值
	p->coef = c;  
	p->expon = e;
	p->link = NULL;
	//将新申请的节点p插入到Rear的后面
	(*pRear)->link = p;
	//同时将Rear向后移一位到新插入的p节点处
	*pRear = p;
}

//传入两个链表的头指针p1、p2，进行多项式相加后返回结果链表的头指针
PNode Add(PNode p1, PNode p2)
{
	int e; double c;
	PNode t1, t2, p, rear,t;
	
	//为了方便链表操作，新建一个空的头结点
	t1 = p1; t2 = p2;      //t1、t2用来分别遍历链表1、2
	p = (PNode)malloc(sizeof(Node));
	p->link = NULL;
	rear = p;

	//对多项式进行相加操作，结果存入链表p中
	while(t1 && t2){      //两多项式链表当前项均不为空时便进入循环
		if (t1->expon == t2->expon){     //case 1：两个指数相等时，系数相加
			c = t1->coef + t2->coef;	 
			e = t1->expon;
			if (c > 1e-10 || c < -1e-10 ) {  //c是浮点数类型，不能直接判断是否等于0
				Attach(c,e,&rear);           //系数不为零时将结果存入链表中
			} 
			t1 = t1->link;                   //临时链表指针t1、t2均向后挪一位
			t2 = t2->link;
		} else if (t1->expon > t2->expon) {  //case 2：t1当前项指数比t2当前项大，由于两个
			c = t2->coef;					 //链表指数均为升序排列，所以t1中所有剩余项的
			e = t2->expon;					 //指数都比t2当前项指数大，故可以直接将当前项
			Attach(c,e,&rear);               //放入结果链表中。之后t2向后挪一位。
			t2 = t2->link;
		} else {              //case 3：操作与case 2基本相同，只是t2当前项指数比t1当前项大
			c = t1->coef;
			e = t1->expon;
			Attach(c,e,&rear);
			t1 = t1->link;
		}
	}
	//如果两个链表中t2先遍历完，则将t1中未遍历的剩余项插入结果链表中
	while(t1){
		c = t1->coef;
		e = t1->expon;
		Attach(c,e,&rear);
		t1 = t1->link;
	}
	//如果两个链表中t1先遍历完，则将t2中未遍历的剩余项插入结果链表中
	while(t2){
		c = t2->coef;
		e = t2->expon;
		Attach(c,e,&rear);
		t2 = t2->link;
	}

	t = p;p = p->link; free(t);  //释放空的头结点

	return p;
}

//传入两个链表的头指针p1、p2，进行多项式相乘后返回结果链表的头指针
PNode Mult(PNode p1, PNode p2)
{
	PNode p,rear,t1,t2,t;
	double c;
	int e;

	if(!p1||!p2) return NULL;
	//新建一个链表用于存放相乘后的结果
	t1 = p1; t2 = p2;
	p = (PNode)malloc(sizeof(Node));
	p->link = NULL;
	rear = p;
	//先将p2链表的第一项乘以p1链表的所有项，得到结果存入链表p中
	while(t1) {
		Attach(t1->coef*t2->coef,t1->expon+t2->expon, &rear);
		t1=t1->link;
	}   
	//两重循环使其剩余的项相互相乘，按指数递增的顺序填入链表p中或与p中项相加
	t2=t2->link;
	while( t2 ) {
		t1 = p1; rear = p;  //rear指针用来定位和操作结果链表p
		while( t1 ) {
			c = t1->coef * t2->coef;
			e = t1->expon + t2->expon;
			//通过循环将rear定位到rear->link->expon恰好>=e的位置
			while (rear->link && rear->link->expon < e) {
				rear = rear->link;       
			} 
			//如果两项指数相等，则系数相加
			if (rear->link && e == rear->link->expon) {
				c = c + rear->link->coef;
				if (c > 1e-10 || c < -1e-10) {   //如果系数和不为0，则相加系数后保存
					rear->link->coef = c;
				} else {                         //如果系数和为0，则删除存有该系数的节点
					t = rear->link; 
					rear->link = rear->link->link; 
					free(t);
				}
			} else {                             //如果rear->link->expon大于e，则申请一个
				t = (PNode)malloc(sizeof(Node)); //新节点储存系数、指数，并将其插入rear指
				t->coef = c; t->expon = e;       //向的节点后面
				t->link = rear->link; rear->link = t;
				rear = rear->link;
			}
			t1 = t1->link;
		}
		t2=t2->link;
	}

	t = p; p = p->link; free(t);   //释放空的头结点

	return p;
}

//对最后的结果链表进行输出。遍历链表并打印。
void PrintPoly(PNode p)
{   //如果链表为空则返回0
	if (!p) {
		printf("0");
		return;
	}

	while (p) {
		printf("%.2fx%d", p->coef, p->expon);
		p = p->link;
		if (p && p->coef > 0) {
			printf("+");
		}
	}
	printf("\n");
}