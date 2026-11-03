/*
 * A348317  a(n) = A348150(n) - A002275(n) where A002275(n) = R_n is the
 *          repunit with n times digit 1.
 * https://oeis.org/A348317
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) = A348150(n) – A002275(n) A348150(n) = smallest n‑digit
 *            integer without the digit 0 that is a Niven number (i.e.
 *            divisible by the sum of its digits)
 * Range:     u8 n <= 79, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Validated: standard, pass, 88 terms through n = 9670 (2026-11-01)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>
#include <stdlib.h>
#include <stddef.h>
#include <string.h>

/* Parse a decimal string into a seq_t value.  Works for all allowed seq_t
   types because the arithmetic operators are defined for both
   machine integers and the big‑int class.                                 */
static seq_t seq_from_str(const char *s)
{
    seq_t v = 0;
    while (*s) { v = v*10 + (seq_t)(*s - '0'); ++s; }
    return v;
}

static void inc_one(uint8_t *digits, size_t n)
{
    if (n == 0) return;
    int i = (int)n - 1;
    int carry = 1;
    while (i >= 0 && carry) {
        int v = digits[i] + carry;
        if (v >= 10) {
            digits[i] = 0;
            carry = 1;
        } else {
            digits[i] = (uint8_t)v;
            carry = 0;
        }
        --i;
    }
    /* if carry remains, the integer would grow past n digits;
       this situation does not arise for the required Niven number. */
}

static void trim_zeros(uint8_t *digits, size_t n)
{
    for (size_t i = 0; i < n; ++i) {
        if (digits[i] == 0) {
            digits[i] = 1;
            for (size_t j = i + 1; j < n; ++j)
                digits[j] = 1;
            break;
        }
    }
}

/* one’s digit array must contain only values 1…9 */

seq_t a(idx_t n)
{
    /* n ≥ 1 by specification, but guard small n */
    if (n == 0) return (seq_t)0;

    size_t len = (size_t)n;
    uint8_t *digits = (uint8_t *)malloc(len);
    if (!digits) return (seq_t)0;
    /* start from the repunit R_n */
    for (size_t i = 0; i < len; ++i) digits[i] = 1;

    while (1) {
        /* compute digit sum and remainder modulo that sum */
        unsigned int sum = 0;
        for (size_t i = 0; i < len; ++i) sum += digits[i];

        unsigned int rem = 0;
        for (size_t i = 0; i < len; ++i) {
            rem = (rem * 10 + digits[i]) % sum;
        }

        if (rem == 0) break;                /* found the Niven number */

        inc_one(digits, len);
        trim_zeros(digits, len);
    }

    /* compute (digits) – (repunit) digitwise */
    uint8_t *diff = (uint8_t *)malloc(len);
    if (!diff) { free(digits); return (seq_t)0; }

    int borrow = 0;
    for (int i = (int)len - 1; i >= 0; --i) {
        int cur = digits[i];
        int dec = 1;                    /* repunit digit */
        int d = cur - dec - borrow;
        if (d < 0) {
            d += 10;
            borrow = 1;
        } else
            borrow = 0;
        diff[i] = (uint8_t)d;
    }

    /* convert diff to a decimal string, stripping leading zeros */
    size_t start = 0;
    while (start < len - 1 && diff[start] == 0) ++start;
    size_t str_len = len - start;
    char *buf = (char *)malloc(str_len + 1);
    if (!buf) {
        free(diff);
        free(digits);
        return (seq_t)0;
    }
    for (size_t i = 0; i < str_len; ++i)
        buf[i] = '0' + diff[start + i];
    buf[str_len] = '\0';

    seq_t result = seq_from_str(buf);

    free(buf);
    free(diff);
    free(digits);
    return result;
}
