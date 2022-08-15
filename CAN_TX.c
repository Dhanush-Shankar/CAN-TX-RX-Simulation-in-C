#include<stdio.h>
#include<string.h>
#include<stdlib.h>
void doframe1(char*,char*);
void doCrc(char*);
void doStuffing(char*);
void printFile(char*);
main()
{
	FILE *fp=fopen("CAN_DATA_FRAME.txt","w+");
	fclose(fp);
	int no,n;
	printf("Enter the number of frame to add to file\t");
	scanf("%d",&no);
	for(n=0;n<no;n++)
	{
		char mesg[20]="",crc[15]="";
		char identifier[11]="";
		printf("Enter the 11-bit identifier\t");
		scanf("%s",identifier);
		printf("\nEnter the message\t");
		scanf("%s",mesg);
		doframe1(identifier,mesg);
	}
}
void doframe1(char a[],char b[])
{

	int u,i=0,j=0;
	u=strlen(b);
	char frame1[100]="";
	frame1[i]='0';
	for(i=1;i<=11;i++)
	{
		frame1[i]=a[j++];
	}
	frame1[i]='0';
	frame1[++i]='0';
	frame1[++i]='0';
	if(u==0)
	{
		frame1[++i]='0';
		frame1[++i]='0';
		frame1[++i]='0';
		frame1[++i]='0';
	}
	
	else if(u==8)
	{
		frame1[++i]='0';
		frame1[++i]='0';
		frame1[++i]='0';
		frame1[++i]='1';
	}
	else if(u==16)
	{
		frame1[++i]='0';
		frame1[++i]='0';
		frame1[++i]='1';
		frame1[++i]='0';
	}
	else if(u==24)
	{
		frame1[++i]='0';
		frame1[++i]='0';
		frame1[++i]='1';
		frame1[++i]='1';
	}
	else if(u==32)
	{
		frame1[++i]='0';
		frame1[++i]='1';
		frame1[++i]='0';
		frame1[++i]='0';
	}

	j=0;
	int r=i+u;
	for(++i;i<=r;i++)
	{
		frame1[i]=b[j];
		j++;
	}
	printf("CAN frame upto data field is: ");
	for(i=0;i<strlen(frame1);i++)
	{
		printf("%c",frame1[i]);
	}
	doCrc(frame1);
}
void doCrc(char c[])
{
	int i,j,m,n;
	char crc[20]="",frame2[100]="";
	printf("\nEnter the polynomial\t");
	scanf("%s",crc);
	char encoded[100]="";
	m=strlen(c);
	n=strlen(crc);
	for(i=0;i<m;i++){
		encoded[i]=c[i];
	}
	for(j=m;j<m+n-1;j++)
	{
		encoded[j]='0';
	}
	for(i=0;i<=strlen(encoded)-n;)
	{
		for(j=0;j<n;j++)
			encoded[i+j]=encoded[i+j]==crc[j]?'0':'1';
		for(;i<strlen(encoded)&&encoded[i]!='1';i++);
	}
	printf("The encoded data is: ");
	for(i=0;i<m+n-1;i++)
		printf("%c",encoded[i]);
	printf("\n");
	for(i=0;i<m+n-1;i++)
		frame2[i]=encoded[i]|c[i];
	printf("Transmitted data before stuffing: %s\n",frame2);

	doStuffing(frame2);
}
void doStuffing(char d[])
{
	char stuff0[100]="";
	int i,j,cnt,len,cmt;
	len=strlen(d);
	int length=len;
	cnt=0;j=0;cmt=0;
	for(i=0;i<=length;i++)
	{
		if(d[i]=='1')
		{
			
			cnt++;
			cmt=0;
		}
		else if(d[i]=='0')
		{
			cmt++;
			cnt=0;
		}
		stuff0[j]=d[i];
		j++;
		if(cnt==5&&d[i+1]=='1')
		{
			stuff0[j]='0';
			j++;
			cnt=0;
		}
		if(cmt==5&&d[i+1]=='0')
		{
			stuff0[j]='1';
			j++;
			cmt=0;
		}
	}
	
	//printf("\n%s\n",stuff0);
	int l,ll;
	l=strlen(stuff0);
	ll=l+10;
	for(i=l;i<ll;i++)
	{
		stuff0[i]='1';
	}
	printf("Transmitted data after stuffing : ");
	for(i=0;i<ll;i++)
	{
		printf("%c",stuff0[i]);

	}
	printf("\n");
	printFile(stuff0);
}
void printFile(char e[])
{
	int i,s;
	s=strlen(e);
	FILE *fp;
	fp=fopen("CAN_DATA_FRAME.txt","a+");
	for(i=0;i<s;i++)
	{
		fputc(e[i],fp);
		
	}
	fprintf(fp,"\n");
	fclose(fp);
}
