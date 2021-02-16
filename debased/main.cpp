#include <iostream>
#include <string>
#include <inttypes.h>
#include <cmath>

char b64table[] = "ABCDEFGHIJKLMNOPQRSTUVWXYZ" "abcdefghijklmnopqrstuvwxyz" "0123456789+/";

char b32table[] = "ABCDEFGHIJKLMNOPQRSTUVWXYZ" "234567";

void reversestr(char* str)
{
	char* len = str + strlen(str) - 1;
	char centre;
	while (str < len) {
		centre = *str;
		*str = *len;
		*len = centre;
		str++;
		len--;
	}
}

void inttocstr(uint32_t value, char* buffer, uint32_t base)
{
	char negative = value >> 31;
	if (!value) {
		buffer[0] = '0';
		buffer[1] = '\0';
	}
	int i = 0;
	switch (base) {
	case 32:
		for (; value; i++) {
			int remainder = value % base;
			buffer[i] = b32table[remainder];
			value /= base;
		}
		break;
	case 64:
		for (; value; i++) {
			int remainder = value % base;
			buffer[i] = b64table[remainder];
			value /= base;
		}
	break;
	case 256:
		for (; value; i++) {
			int remainder = value % base;
			buffer[i] = remainder;
			value /= base;
		}
	default:		
		for(; value; i++){
			int remainder = value % base;
			buffer[i] = (remainder > 9) ? (remainder-10) + 'a': remainder + '0';
			value /= base;
		}
	}
	if (negative)
		buffer[i++] = '-';
	buffer[i] = '\0';
	reversestr(buffer);
}

char assumebase(char* str) {
	bool hasalpha = false; // only hex, 32, and 64
	bool haslower = false;
	bool has8or9 = false; //since base32 and octal don't have 8
	bool has47 = false;
	bool has23 = false;
	bool hasgreater = false;
	if (*str == '0') {
		switch (*(str + 1)) {
		case '0':
		case '1':
		case 'b':
		case 'B':
			return 2;
			break;
		case 'd':
		case 'D':
			return 10;
			break;
		case 'x':
		case 'X':
			return 16;
			break;
		case 'a':
		case 'A':
			return 256;
			break;
		default:
			return 8;
		}
	}
	while(*str != '\0') {
		if (islower(*str)) {
			haslower = 1;
			hasalpha = 1;
		}
		else if (isalpha(*str))
			hasalpha = 1;

		else if(isdigit(*str)){
			switch (*str) {
			case '9':
			case '8':
				has8or9 = 1;
				break;
			case '7':
			case '6':
			case '4':
				has47 = 1;
				break;
			case '3':
			case '2':
				has23 = 1;
				break;
			}
		}
		else if (*str == '/' || *str == '+')
			return 64;
		str++;
	}
	if(!hasalpha) { // 2 4 8 10
		if (has8or9)
			return 10;
		if (has47)
			return 8;
		if (has23)
			return 4;
		return 2;
	}
	if(hasalpha){
		if (haslower)
			return 64;
		if (has8or9)
			return 16;
		return 32;
	}
	return 0;
}

int main() 
{
	char loc[1024];
	
	std::cin >> loc;
	char base = assumebase(loc);
	
	printf("assuming input is base %d...\n", base);
	int value = 0;
	if(base <= 32)
		value = std::stoi(loc[0] == '0' ? loc + 2 : loc, nullptr, base);
	else if (base == 64)
		return printf("base64 strings are not supported, exiting...");
		//value = b64toint()
	else
		return 0;
 
	for (int i = 2; i <= 64; i++) {
		inttocstr(value, loc, i);
		switch (i) {
		case 2:
			printf("0b%s\n", loc);
			break;
		case 4:
			printf("%s\n", loc);
			break;
		case 8:
			printf("0%s\n", loc);
			break;
		case 10:
			printf("0d%s\n", loc);
			break;
		case 16:
			printf("0x%s\n", loc);
			break;
		case 32:
			printf("%s\n", loc);
			break;
		case 64:
			printf("%s\n", loc);
			break;
		}
	}
		inttocstr(value, loc, 256);
		printf("%s\n", loc);
}