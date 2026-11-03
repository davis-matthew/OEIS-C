/*
 * A242350  Multiply a(n-1) by 2 and drop all 0's.
 * https://oeis.org/A242350
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 n <= 8, u16 n <= 20, u32 n <= 600, u64 n <= 600,
 *            u128 n <= 600, big any n
 * Validated: standard, pass, 59 terms through n = 476 (2026-11-03)
 * Helpers:   mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Recurrence: a(1)=1, a(n)=drop_zeroes(2*a(n-1)) */
// Drops all decimal 0's after multiplying by 2

#include <stdlib.h>
#include <stddef.h>
#include "../utils/mem.h"

/* Helper that removes all decimal zeros from x  */
static seq_t drop_zeroes(seq_t x)
{
    seq_t t = x;
    if (t == 0) return 0;                      /* never happens for the sequence */
    /* Find number of decimal digits */
    size_t len = 0;
    seq_t tmp = t;
    while (tmp > 0) {
        ++len;
        tmp /= 10;
    }
    /* Store digits in reverse (least‑significant first) */
    uint8_t *digits = (uint8_t*)malloc(len);
    if (!digits) abort();
    tmp = t;
    for (size_t i = 0; i < len; ++i) {
        digits[i] = (uint8_t)(tmp % 10);
        tmp /= 10;
    }
    /* Reconstruct number ignoring zeros, preserving order */
    seq_t res = 0;
    for (size_t i = len; i > 0; --i) {
        uint8_t d = digits[i-1];
        if (d != 0) {
            res = res * 10 + d;
        }
    }
    free(digits);
    return res;
}

/* Cache for previously computed values.  */
static seq_t *a_cache = NULL;
static idx_t a_cache_size = 0;   /* actual allocated array size (>=1) */
static idx_t a_computed = 0;     /* largest n for which a_cache[n] is valid */

seq_t a(idx_t n)
{
    if (n < 1) return 0; /* undefined, but guard against bad input */

    /* Allocate or grow the cache as needed */
    if (a_cache == NULL) {
        a_cache_size = n + 1;                 /* index 0 is unused */
        a_cache = mem_new(seq_t, a_cache_size);
        if (!a_cache) abort();
        a_cache[1] = 1;
        a_computed = 1;
    } else if ((idx_t)(n + 1) > a_cache_size) {
        idx_t new_size = n + 1;
        seq_t *tmp = mem_resize(seq_t, a_cache, new_size);
        if (!tmp) abort();
        /* zero‑initialize new cells so we can detect uncomputed values */
        for (idx_t i = a_cache_size; i < new_size; ++i) tmp[i] = 0;
        a_cache = tmp;
        a_cache_size = new_size;
    }

    /* Compute missing values up to n */
    for (idx_t i = a_computed + 1; i <= n; ++i) {
        a_cache[i] = drop_zeroes(a_cache[i-1] << 1);
    }
    a_computed = (a_computed > n) ? a_computed : n;

    return a_cache[n];
}
