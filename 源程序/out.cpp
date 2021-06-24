#include <stdio.h> 
#include <stdlib.h> 
typedef	int	opppopppo;
  int  i,  j,  num;
  int  a[20];
  void write(int a)
{
printf("%d",a);
}
void write(char c)
{
printf("%c",c);
}
void read(int &a)
{
scanf("%d",&a);
}
void read(char &c)
{
scanf("%c",&c);
}
int  q(int  num)
{
typedef	int	opppopppo;
  int  i,  j,  k;
  int  t;
  i  =  1;
  while  (i  <  num  )
{  j  =  num  -  i  +  1;
  k  =  1;
  while  (k  <  j  )
{  if  (a[k  +  1]  <  a[k]  )
{  t  =  a[k];
  a[k]  =  a[k  +  1];
  a[k  +  1]  =  t  ;
}
else  
{
t  =  0  ;
}
k  =  k  +  1  ;
}
i  =  i  +  1  ;
}  
}
  int main()
{
  read(num);
  i  =  1;
  while  (i  <(num  +  1)  )
{  read(j);
  a[i]  =  j;
  i  =  i  +  1  ;
}
q(num);
  i  =  1;
  while  (i  <(num  +  1)  )
{  write(a[i]);
  i  =  i  +  1  ;
}  
return 0;
}
  