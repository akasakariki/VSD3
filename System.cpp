#include "Common.h"

//�����񔽓]�v���O����
int ReverseChars(char *chars)
{

	if(chars=='\0')return 0;
    char *p, *q;  
	char ans[50];
	char *ans_p;
	ans_p=&ans[0];
    p = q = &chars[0];                 
	while (*q != '\0'){
		*ans_p=*q;
		ans_p++;
		q++;
	}
	ans_p--;
	q--;
    while (q >= p)      
	{
        //putchar(*q--);       
		*chars=*ans_p;
		DebugText("%s",ans_p);
		ans_p--;
		chars++;
		q--;
	}
	*chars='\0';

	
	return 0;
}



//�g���q�E��
int GetExt(char path[],char *ext)
{

    char *p,*q;  



	p=&path[0];
	q=ext;
	int len=0,len2;
	while(*p!='\0'){
		p++;
		len++;
	}
	p--;
	len--;
	len2=len;
	while(1)
	{
		if(len==0)break;
		if(*p=='.')break;
		*ext=*p;
		ext++;
		p--;
		len--;
		
	}
	*ext='\0';
	ext=q;//�A�h���X�ʒu��擪��
	ReverseChars(ext);

	return 0;
}


