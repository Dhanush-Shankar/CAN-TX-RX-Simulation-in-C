/*
Controller Area Network Receiver Module
Author: Dhanush Shankar PG (22409), Sreemathi (22405),Ujwal BN(22430) ,Rohit U(22402) Vemula S(22428).
DOC: 26/10/2019
TOC: 10:00 AM IST
Modifier: None
*/

#include<stdio.h>
#include<string.h>
#define CRCPOLY "1100010110011001"
void dataRead(char [], char[]);
void crcRead(char[], char[]);
int stuffCheck(int, char[]);
int formCheck(int, char[]);
int destuff(char[], int);
int crcCheck(char [], int);
void showData(char [], int);
int main()
{
    int arrLen, z, nof;
    char dataChar[10][200], newArr[200];
    FILE *fp;
    fp = fopen("file.txt", "r");
    printf("Enter Number of Data Frames want to receive: ");
    scanf("%d",&nof);
      for(z = 0; z < nof; z++)
      {
      	printf("\nDATA FRAME %d\n",z+1);
        fscanf(fp, "%s", dataChar[z]);
        printf("\nRecieved Data Frame:\n%s\n", dataChar[z]);
        arrLen = strlen(dataChar[z]);
		if(stuffCheck(arrLen-10, dataChar[z]))// stop if STUFF error
		{
            printf("\nStuff Error in DATA FRAME %d ..!\n",z+1);
            break;
		}
		else if(formCheck(arrLen, dataChar[z])) //stop if FORM error
		{
		    printf("\nForm Error in DATA FRAME %d ..!\n",z+1);
		    break;
		}
		else if (destuff (dataChar[z], arrLen-10)) //de-stuff & CRC error check
        {
            printf("\nCRC Error in DATA FRAME %d ..!\n", z+1);
            break;
        }
      }
    return 0;
}
// Check for STUFF error
int stuffCheck(int len, char dataChar[200])
{
    int i,j,count=0;
    for(i=0; i < len-6; i++)
    {
    	if(dataChar[i] == '1' && dataChar[i+1] == '1' && dataChar[i+2] == '1' && dataChar[i+3] == '1' && dataChar[i+4] == '1' && dataChar[i+5] == '1')
    	{
    			printf("STUFF error\n");
    			return 1;
		}
		if(dataChar[i] == '0' && dataChar[i+1] == '0' && dataChar[i+2] == '0' && dataChar[i+3] == '0' && dataChar[i+4] == '0' && dataChar[i+5] == '0')
    	{
    			printf("STUFF error\n");
    			return 1;
		}
	}
	return 0;
}
// Check for FORM error
int formCheck(int len, char dataChar[200])
{
    int i;
    for(i = strlen(dataChar); i >= (strlen(dataChar)-10); i--)
    {
        if(dataChar[i] == '0')
            return 1;
    }
    return 0;
}

// De-stuffing frame
int destuff(char dataChar[200], int len)
{
    int i,j,count = 0;
    char destuffChar[200] = "";
     for(i = 0; i < strlen(dataChar); i++)
    {
    	destuffChar[i] = dataChar[i];
	}
	for(i=0; i < len-6; i++)
	{
		if(dataChar[i] == '1' && dataChar[i+1] == '1' && dataChar[i+2] == '1' && dataChar[i+3] == '1' && dataChar[i+4] == '1' && dataChar[i+5] == '0')
        {
			for(j = i+5; j < strlen(destuffChar); j++)
    			{
    					destuffChar[j-count] = destuffChar[(j-count)+1];
				}
				count++;
		}

		if(dataChar[i] == '0' && dataChar[i+1] == '0' && dataChar[i+2] == '0' && dataChar[i+3] == '0' && dataChar[i+4] == '0' && dataChar[i+5] == '1')
		{
			for(j = i+5; j < strlen(destuffChar); j++)
    			{
    					destuffChar[j-count] = destuffChar[(j-count)+1];
				}
				count++;
		}
	}
	printf("\nDestuffed Data Frame:\n");
	int r=strlen(destuffChar)-(count-1);
	for( i = 0;i<r;i++)
	{
		printf("%c",destuffChar[i]);
	}

	if(crcCheck(destuffChar, count))//call to CRC check
        return 1;
    else
    {
        showData(destuffChar, count);
        return 0;
    }
}
//to display various fields
void showData(char cleanData[200], int cnt)
{
	char dlcData[4];
    int i,dlc, end = 0;
    printf("\nReceived Error free Destuffed Data Frame:\n");
    for( i = 0; i < (strlen(cleanData)-(cnt-1));i++)
	{
		printf("%c",cleanData[i]);
	}

    printf("\nSOF: %c",cleanData[0]);
    printf("\nArbitration field: ");
    for(i=1; i<13; i++)
        printf("%c",cleanData[i]);

    printf("\nDLC: ");
    for(i=15;i<19;i++)
    {
        printf("%c", cleanData[i]);
        dlcData[i-15] = cleanData[i];
    }
    printf("\nDATA: ");
    dataRead(cleanData, dlcData);
    printf("\nCRC->ACK->EOF: ");
    crcRead(cleanData, dlcData);
    printf("\n\n");
}
// To Display variable DATA with variable length
void dataRead(char arr[200], char dlcData[4])
{
    int i;
    if(!strcmp(dlcData,"0001"))
        for(i=19;i<27;i++)
            printf("%c",arr[i]);
    if(!strcmp(dlcData,"0010"))
        for(i=19;i<35;i++)
            printf("%c",arr[i]);
    if(!strcmp(dlcData,"0011"))
        for(i=19;i<43;i++)
            printf("%c",arr[i]);
    if(!strcmp(dlcData,"0100"))
        for(i=19;i<51;i++)
            printf("%c",arr[i]);
}
// To display CRC->ACK->EOF
void crcRead(char arr[200], char dlcData[4])
{
    int i;
    if(!strcmp(dlcData,"0001"))
        for(i=27; i<52; i++)
            printf("%c",arr[i]);
    if(!strcmp(dlcData,"0010"))
        for(i=35; i<60; i++)
            printf("%c",arr[i]);
    if(!strcmp(dlcData,"0011"))
        for(i=43; i<68; i++)
            printf("%c",arr[i]);
    if(!strcmp(dlcData,"0100"))
        for(i=51; i<76; i++)
            printf("%c",arr[i]);
}

//To generate CRC & CRC sequence Check
int crcCheck(char dataChar[200], int cnt)
{
	int i,j,count = 0;
	int r=(strlen(dataChar)-(cnt-1))-10;
	char encoded[100] = "";
	for(i = 0; i < r; i++)
		encoded[i] = dataChar[i];
	printf("\n\nSOF to CRC field:\n%s\n",encoded);
	//printf("\nEnter polynomial to generate CRC: ");
	//scanf("%s",crc);
	for(i=0;i<=strlen(encoded)-strlen(CRCPOLY);){
		for(j=0;j<strlen(CRCPOLY);j++)
			encoded[i+j]=encoded[i+j]==CRCPOLY[j]?'0':'1';
		for(;i<strlen(encoded) && encoded[i]!='1';i++);
	}
	for(i=0;i<strlen(encoded);i++)
		if(encoded[i]=='0')
			count++;
	if(count==strlen(encoded))
    {
		return 0;
    }
	else
    {
		printf("\nCRC ERROR..!\n");
		return 1;
    }
}
