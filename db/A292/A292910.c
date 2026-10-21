/*
 * A292910  E.g.f.: exp(1/3! * x^3 * exp(-x)).
 * https://oeis.org/A292910
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign
 * Formula:   a(n) = sum_{k=3}^{n} (-1)^{k-3} * binom(n-1,k-1) * binom(k,3) *
 *            a(n-k) with a(0)=1, a(1)=0, a(2)=0
 * Range:     i8 n <= 7, i16 n <= 11, i32 n <= 16, i64 n <= 24, i128 n <= 40,
 *            big any n
 * Helpers:   binom
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>
#include "../utils/binom.h"

seq_t a(idx_t n) {
    /* static cache to support arbitrary call order and avoid recomputation */
    static seq_t *cache = NULL;
    static size_t cap   = 0;          /* allocated size (in elements) */
    static idx_t high  = 2;           /* highest index already computed (>=2) */

    /* first‑time allocation and initialisation */
    if (!cache) {
        cap = 16;                     /* initial capacity */
        cache = (seq_t *)realloc(NULL, cap * sizeof(seq_t));
        if (!cache) exit(1);
        cache[0] = (seq_t)1;
        cache[1] = (seq_t)0;
        cache[2] = (seq_t)0;
    }

    /* extend cache if necessary */
    if ((idx_t)(n + 1) > cap) {
        size_t newcap = cap * 2;
        if (newcap < (size_t)(n + 1)) newcap = (size_t)(n + 1);
        seq_t *tmp = (seq_t *)realloc(cache, newcap * sizeof(seq_t));
        if (!tmp) exit(1);
        cache = tmp;
        cap = newcap;
    }

    /* nothing to compute if we already have the value */
    if (n <= high)
        return cache[n];

    /* compute missing terms up to n */
    for (idx_t i = high + 1; i <= n; ++i) {
        seq_t sum = (seq_t)0;
        for (idx_t k = 3; k <= i; ++k) {
            seq_t b1 = binom(i - 1, k - 1);   /* binom(i-1, k-1) */
            seq_t b2 = binom(k, 3);           /* binom(k,3) */
            seq_t term = b1 * b2;             /* product of binomials */
            if (((k - 3) & 1) != 0)           /* (-1)^{k-3} when odd */
                term = -term;
            sum += term * cache[i - k];
        }
        cache[i] = sum;
    }

    high = n;
    return cache[n];
}
