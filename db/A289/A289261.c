/*
 * A289261  Coefficients in the expansion of 1/([r]-[2r]x+[3r]x^2-...); [
 *          ]=floor, r=13/8.
 * https://oeis.org/A289261
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 8, u16 n <= 18, u32 n <= 38, u64 n <= 77, u128 n <= 155,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  
 * Sequence a(n) = coefficients of 1/(⌊r⌋ - ⌊2r⌋x + ⌊3r⌋x² - …) with r = 13/8.
 * The generating function is  
 *   (1 - x)(1 + x)²(1 + x²)(1 + x⁴) / (1 - 2x + x² - 2x³ + 2x⁴ - x⁵ + 2x⁶ - 2x⁷).
 * This yields the linear recurrence
 *   a(n) = 2a(n‑1) - a(n‑2) + 2a(n‑3) - 2a(n‑4) + a(n‑5) - 2a(n‑6) + 2a(n‑7)
 * for all n ≥ 10, with initial values
 *   a(0)=1, a(1)=3, a(2)=5, a(3)=9, a(4)=17, a(5)=30, a(6)=52,
 *   a(7)=91, a(8)=160, a(9)=281.
 * The implementation uses memoisation to answer queries for any n
 * in O(n) time per largest n requested, with constant additional space.
 */

#include <stdlib.h>

seq_t a(idx_t n) {
    /* memoisation cache */
    static seq_t *cache = NULL;
    static size_t cap = 0;
    static idx_t len = 0;

    /* have these values precomputed up to a(9) */
    static const seq_t init[10] = {
        (seq_t)1,
        (seq_t)3,
        (seq_t)5,
        (seq_t)9,
        (seq_t)17,
        (seq_t)30,
        (seq_t)52,
        (seq_t)91,
        (seq_t)160,
        (seq_t)281
    };

    /* create cache on first call */
    if (len == 0) {
        cap = 16;
        cache = (seq_t *)malloc(cap * sizeof(seq_t));
        if (!cache) return (seq_t)0; /* out‑of‑memory guard */
        for (size_t i = 0; i < 10; ++i)
            cache[i] = init[i];
        len = 10;
    }

    /* if requested index already computed, just return it */
    if (n < len)
        return cache[(size_t)n];

    /* grow cache if necessary */
    if ((size_t)n >= cap) {
        size_t newcap = cap;
        while ((size_t)n >= newcap)
            newcap *= 2;
        seq_t *newptr = (seq_t *)realloc(cache, newcap * sizeof(seq_t));
        if (!newptr) return (seq_t)0; /* out‑of‑memory guard */
        cache = newptr;
        cap = newcap;
    }

    /* compute new terms using the recurrence (b(n)=0 for n≥10) */
    for (idx_t i = len; i <= n; ++i) {
        /* recurrence only applies for i ≥ 10; the first ten indices are already set */
        if (i < 10) {
            cache[(size_t)i] = init[(size_t)i];
            continue;
        }

        seq_t p = (cache[(size_t)(i-1)] << 1)         /* 2*a(i-1) */
                + (cache[(size_t)(i-3)] << 1)         /* 2*a(i-3) */
                + cache[(size_t)(i-5)]               /*   a(i-5) */
                + (cache[(size_t)(i-7)] << 1);        /* 2*a(i-7) */

        seq_t q = cache[(size_t)(i-2)]               /*   a(i-2) */
                + (cache[(size_t)(i-4)] << 1)         /* 2*a(i-4) */
                + (cache[(size_t)(i-6)] << 1);        /* 2*a(i-6) */

        cache[(size_t)i] = p - q;
    }
    len = n + 1;

    return cache[(size_t)n];
}
