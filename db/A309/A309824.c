/*
 * A309824  Digits of the 10-adic integer (2345678987654321/(1-10^16))^(1/3).
 * https://oeis.org/A309824
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 n <= 10000, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Recurrence: the digits a(n) are the 10‑adic cube root of
   (2345678987654321)/(1-10^16).  They are computed iteratively:
   a(0) = 1, and for n>0 the digit d = a(n) satisfies
   (previous x + d·10^n)^3 ≡ target cube modulo 10^(n+1),
   where the target cube has period 16 given by block
   1234567898765432 (least‑significant digit first).  The
   algorithm maintains x, x² and x³ as arrays of decimal digits
   and updates them at each step.  This yields correct
   a(n) for all n ≥ 0. */

#include <stddef.h>
#include <stdint.h>
#include <stdbool.h>
#include <stdlib.h>
#include <string.h>

/* repeating block of the 10‑adic integer's cube
   (least‑significant digit first) */
static const unsigned char block[16] = {
    1, 2, 3, 4, 5, 6, 7, 8, 9, 8, 7, 6, 5, 4, 3, 2
};

/* global storage for digits */
static unsigned char *digits_cache = NULL;
static size_t          digits_len  = 0, digits_cap  = 0;

static unsigned char *x_digits  = NULL;
static size_t          len_x      = 0, cap_x      = 0;

static unsigned char *x2_digits = NULL;
static size_t          len_x2     = 0, cap_x2     = 0;

static unsigned char *x3_digits = NULL;
static size_t          len_x3     = 0, cap_x3     = 0;

/* ensure capacity of a digit array */
static void ensure_capacity(unsigned char **arr, size_t *len, size_t *cap,
                            size_t needed)
{
    if (needed <= *cap) return;
    size_t new_cap = (*cap) ? (*cap) << 1 : 16;
    while (new_cap < needed) new_cap <<= 1;
    unsigned char *new_arr = (unsigned char *)realloc(*arr, new_cap);
    if (!new_arr) exit(1);
    memset(new_arr + *cap, 0, new_cap - *cap);
    *arr = new_arr;
    *cap = new_cap;
}

/* add a constant to a digit array (little‑endian) */
static void add_const(unsigned char **target, size_t *len_t, size_t *cap_t,
                      unsigned int val, size_t offset)
{
    unsigned int carry = val;
    size_t idx = offset;
    while (carry) {
        ensure_capacity(target, len_t, cap_t, idx + 1);
        unsigned int sum = (*target)[idx] + carry;
        (*target)[idx] = sum % 10;
        carry = sum / 10;
        ++idx;
    }
    if (idx > *len_t) *len_t = idx;
}

/* add scaled source array to target, source is multiplied by factor
   before addition.  source length is len_s, addition starts at offset. */
static void add_scaled(unsigned char **target, size_t *len_t, size_t *cap_t,
                       const unsigned char *source, size_t len_s,
                       unsigned int factor, size_t offset)
{
    ensure_capacity(target, len_t, cap_t, offset + len_s);
    unsigned int carry = 0;
    for (size_t i = 0; i < len_s; ++i) {
        size_t idx = offset + i;
        unsigned int sum = (*target)[idx] + source[i] * factor + carry;
        (*target)[idx] = sum % 10;
        carry = sum / 10;
    }
    size_t idx = offset + len_s;
    while (carry) {
        ensure_capacity(target, len_t, cap_t, idx + 1);
        unsigned int sum = (*target)[idx] + carry;
        (*target)[idx] = sum % 10;
        carry = sum / 10;
        ++idx;
    }
    if (idx > *len_t) *len_t = idx;
}

/* compute all digits up to index n (inclusive) */
static void compute_up_to(size_t n)
{
    while (digits_len <= n) {
        size_t k = digits_len; /* position to compute */

        unsigned char target_digit = block[k % 16];
        unsigned char c_k_old = (k < len_x3) ? x3_digits[k] : 0;

        int diff = (int)target_digit - (int)c_k_old;
        diff %= 10;
        if (diff < 0) diff += 10;
        unsigned char d = (unsigned char)((diff * 7) % 10); /* inverse of 3 mod 10 */

        /* store new digit */
        if (k >= digits_cap) {
            size_t new_cap = digits_cap ? digits_cap << 1 : 16;
            while (new_cap <= k) new_cap <<= 1;
            digits_cache = (unsigned char *)realloc(digits_cache, new_cap);
            if (!digits_cache) exit(1);
            memset(digits_cache + digits_cap, 0, new_cap - digits_cap);
            digits_cap = new_cap;
        }
        digits_cache[k] = d;
        ++digits_len; /* digits_len = k+1 */

        /* update x³ */
        add_scaled(&x3_digits, &len_x3, &cap_x3, x2_digits, len_x2,
                   3 * d, k);                /* 3·x²·d·10^k */
        add_scaled(&x3_digits, &len_x3, &cap_x3, x_digits, len_x,
                   3 * d * d, 2 * k);          /* 3·x·d²·10^{2k} */
        add_const(&x3_digits, &len_x3, &cap_x3, d * d * d, 3 * k); /* d³·10^{3k} */

        /* add new digit to x */
        if (k >= cap_x) {
            size_t new_cap = cap_x ? cap_x << 1 : 16;
            while (new_cap <= k) new_cap <<= 1;
            x_digits = (unsigned char *)realloc(x_digits, new_cap);
            if (!x_digits) exit(1);
            memset(x_digits + cap_x, 0, new_cap - cap_x);
            cap_x = new_cap;
        }
        x_digits[k] = d;
        if (len_x <= k) len_x = k + 1;

        /* update x² using old x (first k digits) */
        add_scaled(&x2_digits, &len_x2, &cap_x2, x_digits, k, 2 * d, k); /* 2·x·d·10^k */
        add_const(&x2_digits, &len_x2, &cap_x2, d * d, 2 * k);          /* d²·10^{2k} */
    }
}

/* public sequence function */
seq_t a(idx_t n)
{
    /* lazy initialization */
    static bool initialized = false;
    if (!initialized) {
        /* initial digit a(0) = 1 */
        digits_cap = 16;
        digits_cache = (unsigned char *)calloc(digits_cap, 1);
        digits_cache[0] = 1;
        digits_len = 1;

        cap_x = 16;
        x_digits = (unsigned char *)calloc(cap_x, 1);
        x_digits[0] = 1;
        len_x = 1;

        cap_x2 = 32;
        x2_digits = (unsigned char *)calloc(cap_x2, 1);
        x2_digits[0] = 1;
        len_x2 = 1;

        cap_x3 = 32;
        x3_digits = (unsigned char *)calloc(cap_x3, 1);
        x3_digits[0] = 1;
        len_x3 = 1;

        initialized = true;
    }

    if ((size_t)n >= digits_len) {
        compute_up_to((size_t)n);
    }
    return (seq_t)digits_cache[(size_t)n];
}
