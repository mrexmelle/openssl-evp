
#ifndef __OPENSSL_AES_H__
#define __OPENSSL_AES_H__

/**
  AES encryption/decryption demo program using OpenSSL EVP apis
  gcc -Wall openssl_aes.c -lcrypto

  this is public domain code. 

  Saju Pillai (saju.pillai@gmail.com)
**/

#include <openssl/evp.h>

#ifdef __cplusplus
extern "C"
{
#endif // __cplusplus

/**
 * Create an 256 bit key and IV using the supplied key_data. salt can be added for taste.
 * Fills in the encryption and decryption ctx objects and returns 0 on success
 **/
extern int aes_init(unsigned char *key_data, int key_data_len, unsigned char *salt, EVP_CIPHER_CTX *e_ctx, 
             EVP_CIPHER_CTX *d_ctx);

/*
 * Encrypt *len bytes of data
 * All data going in & out is considered binary (unsigned char[])
 */
extern unsigned char *aes_encrypt(EVP_CIPHER_CTX *e, unsigned char *plaintext, int *len);

/*
 * Decrypt *len bytes of ciphertext
 */
extern unsigned char *aes_decrypt(EVP_CIPHER_CTX *e, unsigned char *ciphertext, int *len);

#ifdef __cplusplus
}
#endif // __cplusplus

#endif // __OPENSSL_AES_H__

