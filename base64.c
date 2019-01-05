#include "base64.h"

#include <string.h>

// base64 ת����, ��64��
static const char base64_alphabet[] = {
	'A', 'B', 'C', 'D', 'E', 'F', 'G',
	'H', 'I', 'J', 'K', 'L', 'M', 'N',
	'O', 'P', 'Q', 'R', 'S', 'T',
	'U', 'V', 'W', 'X', 'Y', 'Z',
	'a', 'b', 'c', 'd', 'e', 'f', 'g',
	'h', 'i', 'j', 'k', 'l', 'm', 'n',
	'o', 'p', 'q', 'r', 's', 't',
	'u', 'v', 'w', 'x', 'y', 'z',
	'0', '1', '2', '3', '4', '5', '6', '7', '8', '9',
	'+', '/' };

// ����ʱʹ��
static const unsigned char base64_suffix_map[256] = {
	255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 253, 255,
	255, 253, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255,
	255, 255, 255, 255, 255, 255, 255, 255, 253, 255, 255, 255,
	255, 255, 255, 255, 255, 255, 255,  62, 255, 255, 255,  63,
	52,  53,  54,  55,  56,  57,  58,  59,  60,  61, 255, 255,
	255, 254, 255, 255, 255,   0,   1,   2,   3,   4,   5,   6,
	7,   8,   9,  10,  11,  12,  13,  14,  15,  16,  17,  18,
	19,  20,  21,  22,  23,  24,  25, 255, 255, 255, 255, 255,
	255,  26,  27,  28,  29,  30,  31,  32,  33,  34,  35,  36,
	37,  38,  39,  40,  41,  42,  43,  44,  45,  46,  47,  48,
	49,  50,  51, 255, 255, 255, 255, 255, 255, 255, 255, 255,
	255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255,
	255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255,
	255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255,
	255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255,
	255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255,
	255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255,
	255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255,
	255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255,
	255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255,
	255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255,
	255, 255, 255, 255 };


#define sefe_url_char_encode(c) ( (c) == '+' ? '-' : ((c) == '/' ? '_' : (c)) )
#define sefe_url_char_decode(c) ( (c) == '-' ? '+' : ((c) == '_' ? '/' : (c)) )

static char cmove_bits(unsigned char src, unsigned lnum, unsigned rnum) {
	src <<= lnum; // src = src << lnum;
	src >>= rnum; // src = src >> rnum;
	return src;
}

void base64_encode(unsigned char *indata, int inlen, char *outdata, int *outlen, int safe)
{
	if (!indata || inlen <= 0 || !outdata || !outlen) {
		return;
	}

	int in_len = 0; // Դ�ַ�������, ���in_len����3�ı���, ��ô��Ҫ����3�ı���
	int pad_num = 0; // ��Ҫ������ַ�����, ����ֻ��2, 1, 0(0�Ļ�����Ҫƴ��, )
	if (inlen % 3 != 0) {
		pad_num = 3 - inlen % 3;
	}
	in_len = inlen + pad_num; // ƴ�Ӻ�ĳ���, ʵ�ʱ�����Ҫ�ĳ���(3�ı���)

	int out_len = in_len * 4 / 3; // �����ĳ���

	char *p = outdata; // ����ָ��ָ�򴫳�data���׵�ַ

	//����, ����Ϊ������ĳ���, 3�ֽ�һ��
	for (int i = 0; i < in_len; i += 3) {
		int value = *indata >> 2; // ��indata��һ���ַ������ƶ�2bit(����2bit)
		char c = base64_alphabet[value]; // ��Ӧbase64ת������ַ�
		*p = c; // ����Ӧ�ַ�(������ַ�)��ֵ��outdata��һ�ֽ�
		if (safe) {
			*p = sefe_url_char_encode(*p);
		}
		//�������һ��(���3�ֽ�)������
		if (i == inlen + pad_num - 3 && pad_num != 0) {
			if (pad_num == 1) {
				*(p + 1) = base64_alphabet[(int)(cmove_bits(*indata, 6, 2) + cmove_bits(*(indata + 1), 0, 4))];
				*(p + 2) = base64_alphabet[(int)cmove_bits(*(indata + 1), 4, 2)];
				if (safe) {
					*(p + 1) = sefe_url_char_encode(*(p + 1));
					*(p + 2) = sefe_url_char_encode(*(p + 2));
					out_len -= 1;
				}
				else {
					*(p + 3) = '=';
				}
			}
			else if (pad_num == 2) { // ����������Ҫ������ '='
				*(p + 1) = base64_alphabet[(int)cmove_bits(*indata, 6, 2)];
				if (safe) {
					*(p + 1) = sefe_url_char_encode(*(p + 1));
					out_len -= 2;
				}
				else {
					*(p + 2) = '=';
					*(p + 3) = '=';
				}
			}
			break;
		}
		else { // ����������3�ֽڵ�����
			*(p + 1) = base64_alphabet[cmove_bits(*indata, 6, 2) + cmove_bits(*(indata + 1), 0, 4)];
			*(p + 2) = base64_alphabet[cmove_bits(*(indata + 1), 4, 2) + cmove_bits(*(indata + 2), 0, 6)];
			*(p + 3) = base64_alphabet[*(indata + 2) & 0x3f];
			if (safe) {
				*(p + 1) = sefe_url_char_encode(*(p + 1));
				*(p + 2) = sefe_url_char_encode(*(p + 2));
				*(p + 3) = sefe_url_char_encode(*(p + 3));
			}
		}

		p += 4;
		indata += 3;
	}

	*outlen = out_len;
	return;
}

void base64_encode_str(char *indata, char *outdata, int *outlen, int safe)
{
	base64_decode(indata, (int)strlen(indata), outdata, outlen, safe);
	return;
}

void base64_decode(char *indata, int inlen, unsigned char *outdata, int *outlen, int safe)
{
	int t = 0, x = 0, y = 0, i = 0;
	unsigned char c = 0;
	int g = 3;
	int rlen;

	if (!indata || inlen <= 0 || !outdata || !outlen) {
		return ;
	}
	if (!safe && inlen % 4 != 0) { // ��Ҫ��������ݲ���4�ֽڱ���
		return;
	}

	rlen = inlen;
	if (safe) {
		inlen = inlen % 4 == 0 ? inlen : (inlen + 4 - inlen % 4);
	}

	for (x = 0; x < inlen; ++x)
	{
		// ��Ҫ��������ݶ�Ӧ��ASCIIֵ��Ӧbase64_suffix_map��ֵ
		c = x < rlen ? indata[x] : '=';
		if (safe) {
			c = sefe_url_char_decode(c);
		}
		c = base64_suffix_map[c];
		if (c == 255) {
			return;// ��Ӧ��ֵ����ת�����
		}
		if (c == 253) {
			continue;// ��Ӧ��ֵ�ǻ��л��߻س�
		}
		if (c == 254) {  // ��Ӧ��ֵ��'='
			c = 0;
			g--;
		}
		t = (t << 6) | c; // �������η���һ��int����ռ3�ֽ�
		if (++y == 4) {
			outdata[i++] = (unsigned char)((t >> 16) & 0xff);
			if (g > 1) {
				outdata[i++] = (unsigned char)((t >> 8) & 0xff);
			}
			if (g > 2) {
				outdata[i++] = (unsigned char)(t & 0xff);
			}
			y = t = 0;
		}
	}

	*outlen = i;

	return ;
}

void base64_decode_str(char *indata, int inlen, unsigned char *outdata, int *outlen, int safe)
{
	base64_decode(indata, (int)strlen(indata), outdata, outlen, safe);
	return;
}