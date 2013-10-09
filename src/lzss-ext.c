#include <ruby.h>
#include <ctype.h>
#include <stdlib.h>

// from lzss.c
int decompress_lzss(u_int8_t **dst, u_int8_t *src, u_int32_t srclen);
u_int8_t *compress_lzss(u_int8_t *dst, u_int32_t dstlen, u_int8_t *src, u_int32_t srcLen);

static VALUE encode(VALUE self, VALUE str) {
	u_int32_t srclen = RSTRING_LEN(str);
	u_int8_t *dst = (u_int8_t *) malloc(srclen * 2);
	u_int8_t *end = compress_lzss(dst, srclen * 2, 
		RSTRING_PTR(str), RSTRING_LEN(str));
	
	VALUE ret = Qnil;

	if (end) {
		int compressed_len = end - dst;
		ret = rb_str_new(dst, compressed_len);
	}

	free(dst);
	return ret;
}

static VALUE decode(VALUE self, VALUE str) {
	u_int8_t *dst = NULL;
	u_int32_t len = decompress_lzss(&dst, RSTRING_PTR(str), RSTRING_LEN(str));
	VALUE ret = rb_str_new(dst, len);
	free(dst);
	return ret;
}

void Init_lzss() {
	VALUE Compress = rb_define_module("Compress");
	VALUE LZSS = rb_define_module_under(Compress, "LZSS");
	rb_define_module_function(LZSS, "encode", RUBY_METHOD_FUNC(encode), 1);
	rb_define_module_function(LZSS, "decode", RUBY_METHOD_FUNC(decode), 1);
}
