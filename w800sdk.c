#undef LOG_TAG
#define LOG_TAG "w800sdk"

#include <stdint.h>
#include <string.h>

#include "w800sdk_conf.h"
#include "wm_internal_flash.h"
#include "wm_fwup.h"
#include "elog.h"

#define W800_SDK_VER_MAIN     0
#define W800_SDK_VER_SUB      1
#define W800_SDK_VER_REVISION 4

#define _2STR(x) #x
#define CONV_STR(x) _2STR(x)

// --- internal datas

// --- export funcs

int w800sdk_get_version_main(void)
{
    return W800_SDK_VER_MAIN;
}

int w800sdk_get_version_sub(void)
{
    return W800_SDK_VER_SUB;
}

int w800sdk_get_version_revision(void)
{
    return W800_SDK_VER_REVISION;
}

const char *w800sdk_get_version_string(void)
{
    return (CONV_STR(W800_SDK_VER_MAIN) "." CONV_STR(W800_SDK_VER_SUB) "." CONV_STR(W800_SDK_VER_REVISION));
}

int w800sdk_get_img_info(struct IMAGE_HEADER_PARAM *out)
{
#define IMG_HEAD_MAGIC_NO (0xA0FFFF9F)

    tls_fls_read(CONFIG_W800_IMAGE_HEADER_ADDR, (uint8_t *)out, sizeof(struct IMAGE_HEADER_PARAM));

    if (out->magic_no != IMG_HEAD_MAGIC_NO) {
        log_e("img magic '0x%08x' error ! expected '0x%08x'", out->magic_no, IMG_HEAD_MAGIC_NO);
        return -1;
    }

    return WM_SUCCESS;
}

// int w800sdk_get_img_signature(uint8_t buff_128bytes[])
// {
//     struct IMAGE_HEADER_PARAM img_info;

//     if (w800sdk_get_img_info(&img_info) != WM_SUCCESS) {
//         return -1;
//     }

//     if (!img_info.img_attr.b.signature) {
//         log_w("image not has a signature !");
//         return -1;
//     }

//     uint32_t sign_addr = img_info.img_addr + img_info.img_len;
//     return tls_fls_read(sign_addr, buff_128bytes, 128);
// }

// --- flash dummy

#if CONFIG_W800_CODE_ENCRYPT
    #define _IMG_CODE_ENC_ALIGN    16
    #define _IMG_CODE_ENC_KEY_SIZE 256 /* usrkey_enc.bin + pubkey_N.bin */
#else
    #define _IMG_CODE_ENC_ALIGN    0
    #define _IMG_CODE_ENC_KEY_SIZE 0
#endif

#if CONFIG_W800_IMAGE_SIGN
    #define _IMG_CODE_SIGN_KEY_SIZE 128 /* sign.bin */
#else
    #define _IMG_CODE_SIGN_KEY_SIZE 0
#endif

#define _FLASH_DUMMY_SIZE (_IMG_CODE_ENC_ALIGN + _IMG_CODE_ENC_KEY_SIZE + _IMG_CODE_SIGN_KEY_SIZE)

#if _FLASH_DUMMY_SIZE > 0
    static const uint8_t _flash_dummy[_FLASH_DUMMY_SIZE]
        __attribute__((used))
        __attribute__((section(".img_dummy")));
#endif
