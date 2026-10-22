/*
 * A365917  Expansion of e.g.f. 1 / ( 1 - Sum_{k>=0} x^(4*k+5) / (4*k+5)! ).
 * https://oeis.org/A365917
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = Sum_{k=0..floor((n-5)/4)} binomial(n,4*k+5) * a(n-4*k-5).
 *            Initial values: a(0)=1, a(1)=a(2)=a(3)=a(4)=0
 * Range:     u8 n <= 9, u16 n <= 14, u32 n <= 19, u64 n <= 29, u128 n <= 31,
 *            big any n
 * Helpers:   binomial
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <string.h>
#include "../utils/binomial.h"   /* provides seq_t binomial(idx_t a, idx_t b); */

seq_t a(idx_t n)
{
    static seq_t *cache = NULL;   /* cache of computed values */
    static size_t capacity = 0;   /* allocated array length */

    /* ensure cache can hold indices up to n */
    if ((size_t)(n + 1) > capacity) {
        size_t new_cap = (n + 1) * 2;
        if (new_cap < 16) new_cap = 16;
        seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        if (!tmp) abort();          /* out of memory */
        /* zero‑initialize the new part */
        memset(tmp + capacity, 0,
               (new_cap - capacity) * sizeof(seq_t));
        cache = tmp;
        capacity = new_cap;
    }

    /* compute values 0 … n in order, using the recurrence */
    for (idx_t i = 0; i <= n; ++i) {
        if (i < 5) {                /* a(0)=1, a(1..4)=0 */
            cache[i] = (i == 0) ? 1 : 0;
            continue;
        }
        seq_t sum = 0;
        for (idx_t k = 0;; ++k) {
            idx_t m = 5 + 4 * k;
            if (m > i) break;       /* no more terms */
            sum += binomial(i, m) * cache[i - m];
        }
        cache[i] = sum;
    }

    return cache[n];
}
