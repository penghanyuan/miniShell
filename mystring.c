#include "mystring.h"

char * subString(const char * start, const char * end){

    char *result;
    int i,len_start,len_end,len_res;
    len_start = (int)strlen(start);
    len_end = (int)strlen(end);
    len_res = len_start - len_end;
    result = (char*)malloc((size_t)(len_res+1)*sizeof(char));
    for(i = 0;i<len_res;i++){
        result[i] = start[i];
    }

    result[i] = '\0';
    return result;
}

void removeStrRTrim(char *pStr)  
{  
    char *pTmp = pStr+strlen(pStr)-1;  
      
    while (*pTmp == ' ')   
    {  
        *pTmp = '\0';  
        pTmp--;  
    }  
}  
  
/*去除字符串左边空格*/  
void removeStrLTrim(char *pStr)  
{  
    char *pTmp = pStr;  
      
    while (*pTmp == ' ')   
    {  
        pTmp++;  
    }  
    while(*pTmp != '\0')  
    {  
        *pStr = *pTmp;  
        pStr++;  
        pTmp++;  
    }  
    *pStr = '\0';  
}