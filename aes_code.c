#include "aes_code.h"

/* byte转换16进制字符串 */
void byte2hexstr(char *dest, unsigned char *src, int len) {
    int i;
    unsigned char high_byte, low_byte;

    for (i = 0; i < len; i++) {
        high_byte = src[i] >> 4;
        low_byte = src[i] & 0x0f;

        high_byte += 0x30;
        if (high_byte > 0x39) {
            dest[i * 2] = high_byte + 0x07;
        } else {
            dest[i * 2] = high_byte;
        }

        low_byte += 0x30;
        if (low_byte > 0x39) {
            dest[i * 2 + 1] = low_byte + 0x07;
        } else {
            dest[i * 2 + 1] = low_byte;
        }
    }
}

/* 16进制字符串转换成byte */
void hexstr2byte(unsigned char *dest, char *src, int len) {
    int i;
    unsigned char high_byte, low_byte;

    for (i = 0; i < len; i += 2) {
        high_byte = toupper(src[i]);
        low_byte = toupper(src[i + 1]);

        if (high_byte > 0x39) {
            high_byte -= 0x37;
        } else {
            high_byte -= 0x30;
        }

        if (low_byte > 0x39) {
            low_byte -= 0x37;
        } else {
            low_byte -= 0x30;
        }

        dest[i / 2] = (high_byte << 4) | low_byte;
    }
}

/* AES加密 */
char* aes_enc(char *string) {
    char *plain = "Foxconn123$$$$$$$$";
    unsigned char iv[AES_BLOCK_SIZE] = {0};
    unsigned char *output;
    long len;
    AES_KEY key;

    /* 设置密钥 */
    if (AES_set_encrypt_key((const unsigned char *)plain, 128, &key) < 0) {
        fprintf(stderr,  "Unable to set encryption key in AES\n");
    }

    /* 待加密数据长度 */
    if ((strlen(string) + 1) % AES_BLOCK_SIZE == 0) {
        len = strlen(string) + 1;
    } else {
        len = ((strlen(string) + 1) / AES_BLOCK_SIZE + 1) * AES_BLOCK_SIZE;
    }

    /* 动态分配密文串 */
    output = (unsigned char *)calloc(len, sizeof(unsigned char));
    char *pawd = (char *)calloc(len, sizeof(char *));

    /* 加密 */
    AES_cbc_encrypt((unsigned char *)string, output, len, &key, iv, AES_ENCRYPT);

    /* byte转换16进制字符串 */
    byte2hexstr(pawd, output, len);
    
    return pawd;

}

/* AES解密 */
char *aes_dec(char *pawd) {
    char *plain = "Foxconn123$$$$$$$$";
    unsigned char iv[AES_BLOCK_SIZE] = {0};
    unsigned char *input;
    int len;
    AES_KEY key;

    /* 设置密钥 */
    if (AES_set_decrypt_key((const unsigned char *)plain, 128, &key) < 0) {
        fprintf(stderr,  "Unable to set encryption key in AES\n");
    }

    /* 待加密数据长度 */
    len = strlen(pawd);

    /* 态度分配明文串 */
    input = (unsigned char *)calloc(len, sizeof(unsigned char));
    char *string = (char *)calloc(len, sizeof(char *));

    /* 16进制字符串转换成byte */
    hexstr2byte(input, pawd, len);

    /* 解密 */
    AES_cbc_encrypt(input, (unsigned char *)string, len, &key, iv, AES_DECRYPT);
    
    return string;
}
