
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "openssl_aes.h"

int decrypt_aes(const char * aFileName, char * aPlainText)
{
	/* "opaque" encryption, decryption ctx structures that libcrypto uses to record
	status of enc/dec operations */
	EVP_CIPHER_CTX en, de;

	/* 8 bytes to salt the key_data during key generation. This is an example of
	compiled in salt. We just read the bit pattern created by these two 4 byte 
	integers on the stack as 64 bits of contigous salt material - 
	ofcourse this only works if sizeof(int) >= 4 */
	unsigned int salt[] = {0x2404, 0x3009};
	unsigned char key_data[16]=
	{
		0x61,
		0x72,
		0x69,
		0x65,
		0x20,
		0x4B,
		0x4F,
		0x4E,
		0x54,
		0x4F,
		0x4C,
		0x00, 0x00, 0x00, 0x00, 0x00
	};
	int key_data_len=strlen((char*)(key_data));

	/* gen key and iv. init the cipher ctx object */
	if (aes_init(key_data, key_data_len, (unsigned char *)&salt, &en, &de))
	{
		return -1;
	}

	/* encrypt and decrypt each input string and compare with the original */
	char *plaintext;
	unsigned char *ciphertext;
	int lSize;

	/* The enc/dec functions deal with binary data and not C strings. strlen() will 
	return length of the string without counting the '\0' string marker. We always
	pass in the marker byte to the encrypt/decrypt functions so that after decryption 
	we end up with a legal C string */
	FILE * fp=fopen(aFileName, "r");
	if(fp)
	{
		fseek(fp, 0, SEEK_END);
		lSize=ftell(fp);
		rewind(fp);

		ciphertext=(unsigned char*)malloc(lSize+1);
			
		fread(ciphertext, 1, lSize, fp);
		fclose(fp);
	}
	else
	{
		EVP_CIPHER_CTX_cleanup(&de);
		return -2;
	}

	plaintext = (char *)aes_decrypt(&de, ciphertext, &lSize);

	memset(aPlainText, 0x0, lSize+1);
	strcpy(aPlainText, plaintext);

	EVP_CIPHER_CTX_cleanup(&de);
	free(ciphertext);
	free(plaintext);

	return 0;
}

int main(int argc, char * argv[])
{
	if(argc<2)
	{
		printf("Usage: %s <bin file>\n", argv[0]);
		return -1;
	}

	int status;

	char plainText[128];
	memset(plainText, 0x0, 128);

	status=decrypt_aes(argv[1], plainText);
	if(status!=0)
	{
		printf("main - fail decrypt aes: %d\n", status);
		return -1;
	}

	printf("main - decipher result: %s\n", plainText);

	return 0;
}

