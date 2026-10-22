/*
 * A321802  Delete all consecutive identical decimal digits of n; write -1 if
 *          all digits disappear.
 * https://oeis.org/A321802
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign, base
 * Formula:   not stated
 * Range:     i8 n <= 127, i16 n <= 10000, i32 n <= 10000, i64 n <= 10000,
 *            i128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Removes consecutive identical decimal digits of n; returns -1 if all
   digits disappear; leading zeros are removed unless the result is 0. */

#include <stddef.h>   /* for size_t */
#include <stdint.h>   /* for uint8_t */

/* The main function required by the build system */
seq_t a(idx_t n)
{
    /* collect decimal digits of n, most significant first */
    static uint8_t dig[25];
    size_t len = 0;

    if (n == 0) {
        dig[0] = 0;
        len = 1;
    } else {
        idx_t tmp = n;
        while (tmp > 0) {
            dig[len++] = (uint8_t)(tmp % 10);
            tmp /= 10;
        }
        /* reverse to get most significant first */
        for (size_t i = 0; i < len / 2; ++i) {
            uint8_t t = dig[i];
            dig[i] = dig[len - 1 - i];
            dig[len - 1 - i] = t;
        }
    }

    /* keep only digits that are not part of a run of identical digits */
    static uint8_t keep[25];
    size_t klen = 0;
    for (size_t i = 0; i < len; ) {
        size_t j = i + 1;
        while (j < len && dig[j] == dig[i]) ++j;
        if (j - i == 1) {
            keep[klen++] = dig[i];
        }
        i = j;
    }

    if (klen == 0) {
        return (seq_t)-1;
    }

    /* strip leading zeros unless the result is zero */
    size_t p = 0;
    while (p < klen && keep[p] == 0) ++p;
    if (p == klen) {
        return (seq_t)0;
    }

    /* assemble remaining digits into the result */
    seq_t res = 0;
    for (size_t i = p; i < klen; ++i) {
        res = res * 10 + (seq_t)keep[i];
    }

    return res;
}
