/*
 * A334027  "Look and say" the concatenated exponents in the prime
 *          factorization of n.
 * https://oeis.org/A334027
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) = look‑and‑say of the concatenated exponents in the prime
 *            factorisation of n. For n = 1 the exponents string is taken to
 *            be "0". The function works for any integer type seq_t supplied
 *            by the build (unsigned integer or arbitrary‑precision)
 * Range:     u8 n <= 11, u16 n <= 61, u32 n <= 61, u64 n <= 61, u128 n <= 61,
 *            big any n
 * Helpers:   append_uint_buf
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include "../utils/append_uint_buf.h"

/* ------------------------------------------------------------------ */
/* Factorise n -> arrays of exponents.  Limits are generous for n up
   to 64‑bit; dynamic allocation ensures compatibility.          */
/* ------------------------------------------------------------------ */
static void factor_exponents(uint64_t n, uint64_t **exps, size_t *cnt)
{
    uint64_t tmp = n;
    size_t capacity = 8, size = 0;
    uint64_t *arr = (uint64_t *)malloc(capacity * sizeof(uint64_t));
    if (!arr) abort();

    for (uint64_t p = 2; p * p <= tmp; ++p) {
        if (tmp % p == 0) {
            uint64_t e = 0;
            while (tmp % p == 0) {
                tmp /= p;
                ++e;
            }
            if (size == capacity) {
                capacity <<= 1;
                arr = (uint64_t *)realloc(arr, capacity * sizeof(uint64_t));
                if (!arr) abort();
            }
            arr[size++] = e;
        }
    }
    if (tmp > 1) {                     /* final prime factor */
        if (size == capacity) {
            capacity <<= 1;
            arr = (uint64_t *)realloc(arr, capacity * sizeof(uint64_t));
            if (!arr) abort();
        }
        arr[size++] = 1;
    }
    *exps = arr;
    *cnt  = size;
}

/* ------------------------------------------------------------------ */
/* Append decimal digits of val into a uint8_t buffer (most significant
   first).  The buffer is grown as needed.                          */

/* ------------------------------------------------------------------ */
/* Compute the look‑and‑say of a digit buffer.  Input: digits in order
   (most significant first).  Output: result digits in same order.     */
/* ------------------------------------------------------------------ */
static void look_and_say(const uint8_t *in, size_t inlen,
                         uint8_t **out, size_t *outlen)
{
    size_t outcap = 0, outsize = 0;
    uint8_t *buf = NULL;

    size_t i = 0;
    while (i < inlen) {
        uint8_t d = in[i];
        size_t j = i + 1;
        while (j < inlen && in[j] == d) ++j;
        size_t run = j - i;

        /* write run length as decimal digits */
        append_uint_buf((uint64_t)run, &buf, &outsize, &outcap);

        /* write the digit itself */
        if (outsize == outcap) {
            outcap = (outcap == 0) ? 8 : (outcap << 1);
            buf = (uint8_t *)realloc(buf, outcap);
            if (!buf) abort();
        }
        buf[outsize++] = d;

        i = j;
    }

    *out = buf;
    *outlen = outsize;
}

/* ------------------------------------------------------------------ */
/* Main sequence function as required by the build.                   */
/* ------------------------------------------------------------------ */
seq_t a(idx_t n)
{
    /* Handle n == 1 explicitly (exponent string "0") */
    if (n == 1) {
        return (seq_t)10;   /* look and say of "0" is "10" */
    }

    /* ---------------------------------------------------------------- */
    /* 1. Factor n and collect exponents */
    /* ---------------------------------------------------------------- */
    uint64_t *exps = NULL;
    size_t  expsz = 0;
    factor_exponents((uint64_t)n, &exps, &expsz);

    /* ---------------------------------------------------------------- */
    /* 2. Build concatenated exponent digits */
    /* ---------------------------------------------------------------- */
    size_t  exp_len = 0, exp_cap = 0;
    uint8_t *exp_digits = NULL;

    for (size_t k = 0; k < expsz; ++k) {
        append_uint_buf(exps[k], &exp_digits, &exp_len, &exp_cap);
    }

    free(exps);

    /* ---------------------------------------------------------------- */
    /* 3. Apply look‑and‑say */
    /* ---------------------------------------------------------------- */
    size_t res_len = 0;
    uint8_t *res_digits = NULL;
    look_and_say(exp_digits, exp_len, &res_digits, &res_len);

    free(exp_digits);

    /* ---------------------------------------------------------------- */
    /* 4. Convert resulting digits to seq_t value */
    /* ---------------------------------------------------------------- */
    seq_t value = (seq_t)0;
    for (size_t i = 0; i < res_len; ++i) {
        value = value * (seq_t)10 + (seq_t)(res_digits[i]);
    }

    free(res_digits);
    return value;
}
