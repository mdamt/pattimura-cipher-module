/*
 * Pattimura
 * https://bitbucket.org/abeinoe/pattimura/overview
 *
 * Pattimura algorithm is (c) 2015, Abraham Ferdinand
 * 
 * This module is (c) 2015, Mohammad Anwari <mdamt@mdamt.net> 
 *
 * This program is free software; you can redistribute it and/or modify it
 * under the terms of the GNU General Public License as published by the Free
 * Software Foundation; either version 2 of the License, or (at your option)
 * any later version.
 *
 */

#include <linux/string.h>
#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/init.h>
#include <linux/crypto.h>
#include "pattimura.h"

#define MAX_KEY_SIZE 256/8

struct crypto_pattimura_ctx {
    int key_len;
    unsigned char key[MAX_KEY_SIZE];
	PATTIMURA_Context ctx;
};

static int c(const u8 *in, short len) {
    int i;

    for(i=0; i < len; i++)
                printk("%.2x ", in[i]);
    printk("\n");
}

static int crypto_pattimura_setkey(struct crypto_tfm *tfm, const u8 *in_key, unsigned int key_len) {
	struct crypto_pattimura_ctx *ctx = crypto_tfm_ctx(tfm);
	u32 *flags = &tfm->crt_flags;
    switch (key_len) {
        case 16: case 24: case 32:
            break;
        default:
            *flags |= CRYPTO_TFM_RES_BAD_KEY_LEN;
            return -EINVAL;
    }

    memcpy(ctx->key, in_key, key_len);
    //c(ctx->key, key_len);
    ctx->key_len = key_len * 8;

    return 0;
}

static void crypto_pattimura_encrypt(struct crypto_tfm *tfm, u8 *dst, const u8 *src) {
	struct crypto_pattimura_ctx *ctx = crypto_tfm_ctx(tfm);
    short i = 0;

    i = PATTIMURA_Open(&ctx->ctx, ctx->key, ctx->key_len, PATTIMURA_CBC_ENC, PATTIMURA_default_userbox);
    if (i == 0) {
        i = PATTIMURA_EncryptDecript(&ctx->ctx, dst, src, 1);
    }
    if (i) {
        printk("Failed to encrypt %d", i);
    }
}

static void crypto_pattimura_decrypt(struct crypto_tfm *tfm, u8 *dst, const u8 *src) {
	struct crypto_pattimura_ctx *ctx = crypto_tfm_ctx(tfm);
    short i = 0;

    i = PATTIMURA_Open(&ctx->ctx, ctx->key, ctx->key_len, PATTIMURA_CBC_DEC, PATTIMURA_default_userbox);
    if (i == 0) 
    i = PATTIMURA_EncryptDecript(&ctx->ctx, dst, src, 1);
    if (i) {
        printk("Failed to decrypt %d", i);
    }
}

MODULE_LICENSE("GPL");
MODULE_DESCRIPTION("Crypto kernel module with Pattimura algorithm");
MODULE_ALIAS_CRYPTO("crypto_pattimura");

static struct crypto_alg crypto_pattimura_algorithm = {
	.cra_name		=	"pattimura",
	.cra_flags		=	CRYPTO_ALG_TYPE_CIPHER,
	.cra_blocksize		=	PATTIMURA_BLOCKBYTES,
	.cra_ctxsize		=	sizeof (struct crypto_pattimura_ctx),
	.cra_alignmask		=	0,
	.cra_module		=	THIS_MODULE,
	.cra_u			=	{ .cipher = {
	.cia_min_keysize	=	16,
	.cia_max_keysize	=	32,
	.cia_setkey		= 	crypto_pattimura_setkey,
	.cia_encrypt		=	crypto_pattimura_encrypt,
	.cia_decrypt		=	crypto_pattimura_decrypt } }
};


static int __init crypto_pattimura_init(void)
{
    printk("-----------> Init\n");
    return crypto_register_alg(&crypto_pattimura_algorithm);
}

static void __exit crypto_pattimura_cleanup(void)
{
    crypto_unregister_alg(&crypto_pattimura_algorithm);
}

module_init(crypto_pattimura_init);
module_exit(crypto_pattimura_cleanup);
