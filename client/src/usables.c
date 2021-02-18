#include "client.h"

void AddSpace(char *name)
{
    for(int i = strlen(name); i < 16; i++)
    {
        name[i] = ' ';
    }
}

char* addX(char* name) {
    for (unsigned int i = 0; i < strlen(name); i++) {
        if (name[i] == ' ') {
            name[i] = '_';
        }
    }
    return name;
}

char* addY(char* name) {
    for (unsigned int i = 0; i < strlen(name); i++) {
        if (name[i] == '_') {
            name[i] = ' ';
        }
    }
    return name;
}

bool CmpLogsWithoutAddy(char log1[17], char log2[17] ) {
    char _log1[17];
    bzero(_log1, 17);
    char _log2[17];
    bzero(_log2, 17);
    
    for (int i = 0; i < 16; i++) {
        _log1[i] = log1[i];
    }
    addY(_log1);
    return (!strncmp(_log1, log2, 16));
}


void mx_swap_char(char *s1, char *s2)
{
	char temp = *s1;
    *s1 = *s2;
    *s2 = temp;
}

void mx_str_reverse(char *s)
{
	int a = strlen(s);
	for(int i = 0; i < a/2; i++) {
		mx_swap_char(&s[i],&s[a-1-i]);
	}
}

char *mx_strnew(const int size) {
    char *mem = NULL;
    int i = 0;
    if (size < 0)
        return NULL;
    mem = (char *)malloc((size + 1) * sizeof(char));
    while (i < size) {
        mem[i] = '\0';
        i++;
    }
    mem[i] = '\0';
    return mem;
}

int number_length(int id) {
    int l = 0;
    while (id > 0) {
        id = id/10;
        l += 1;
    }

    return l;
}

char *mx_itoa(int number) {
	int length = number_length(number);
	int tmp = number;
	char *result = NULL;
	result = mx_strnew(length);
	if (number == 0) {
		return strcpy(result, "0");
	}
	if (number == -2147483648) {
		return strcpy(result, "-2147483648");
	}
	tmp = number;
	for (int i = 0; i < length; i++) {
		if (tmp < 0) {
			result[length] = '-';
			tmp = -tmp;
		}
		result[i] = (tmp % 10) + '0';
		tmp /= 10;
	}
	mx_str_reverse(result);
	return result;
}

char* addzr(int id) {
    char* idStr = NULL;
    char* str = mx_itoa(id);
    idStr = mx_strnew(5);
    int l = 0;
    l = strlen(str);

    for (int i = 0; i < 4 - l; i++) {
        idStr[i] = '0';
    }
    strcat(idStr, mx_itoa(id));
    idStr[4] = '\0';
    return idStr;
}

void NewDialog() {
    struct ChatsBase*  arr = malloc(chatnumber * sizeof(struct ChatsBase));
    for (int i = 1; i < chatnumber; i++) {
        bzero(arr[i].username, 17);
        bzero(arr[i].chatName, 33);
        
        strncat(arr[i].username, ChatsArray[i - 1].username, 16);
        strncat(arr[i].chatName, ChatsArray[i - 1].chatName, 32);
    }
    ChatsArray = arr;
}

int my_strlen(const unsigned char* s) {
   int i = 0;

   while(s[i]) {
      i++;
   }

   return i;
}
