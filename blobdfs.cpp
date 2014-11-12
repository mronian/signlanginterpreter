int shape=0;
int top=0;
int a[I->height][I->width]={-1};
int stack[I->height*I->width][2]={0};
IplImage* blob(IplImage* I)
{
IplImage* blobimg=cvCreateImage(cvGetSize(img), IP_DEPTH_8U, 3);

ht=I->height;
wd=I->width;
int r, l;

for(r=0;r<ht;r++)
{
	for(l=0;l<wd;l++)
	{
        if (I[r][l]==0)&&(a[r][l]==-1)
            dfs(I,r,l);
	}
}

return a;
}

void push(int a, int b)
{
top=top+1;
stack[top][1]=a;
stack[top][2]=b;
}

void pop()
{
top=top-1;
}

void dfs(IplImage* I,int m, int n)
{
shape=shape+1;
push(m,n);
a[m][n]=0;
chk=0;
int i, j;
while(top>0)
{
    x=stack[top][1];
    y=stack[top][2];
    if(I(x,y)==0)
       {
		a[x][y]=shape/5;
		chk=0;
	}
    for(i=x-1;i<=x+1;i++)
	{
	for(j=y-1;j<=y+1;j++)
    {
		if (I[i][j]==0)&&(a[i][j]==-1)
		{
                push(i,j);
                chk=1;
                break
		}
	}
        if (chk==1)
            break;
	}
    if(chk==0)
       {
		   pop();
        continue;
	}
    if(chk==1)
		continue;
}
}
