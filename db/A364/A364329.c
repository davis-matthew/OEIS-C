/*
 * A364329  G.f. satisfies A(x) = (1 + x^3) * (1 + x*A(x)^2).
 * https://oeis.org/A364329
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 6, u16 n <= 9, u32 n <= 17, u64 n <= 27, u128 n <= 27,
 *            big any n
 * Helpers:   mul_small
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* A364329: a(0)=1, a(1)=1, a(2)=2, a(3)=6, a(4)=17, a(5)=52, a(6)=167,
   a(7)=558, a(8)=1912, a(9)=6683, the rest satisfies
   a(n) = ((4n-2)*a(n-1) - (n+1)*a(n-3) + (12n-42)*a(n-4) + (12n-78)*a(n-7)
           + (4n-38)*a(n-10)) / (n+1)   (n ≥10).  */

#include <stdlib.h>
#include <stddef.h>
#include "../utils/mul_small.h"   /* mul_small(seq_t x, unsigned int m) */

seq_t a(idx_t n)
{
    /* static cache of computed terms */
    static seq_t *cache = NULL;
    static size_t cache_cap = 0;   /* allocated capacity */
    static size_t cache_size = 0;  /* number of terms actually stored */

    /* enlarge cache if necessary */
    if (n + 1 > cache_cap) {
        size_t new_cap = (cache_cap == 0) ? 16 : cache_cap;
        while (new_cap <= n) new_cap <<= 1;
        seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        if (!tmp) exit(EXIT_FAILURE);
        cache = tmp;
        cache_cap = new_cap;
    }

    /* initialise first ten terms (given in the OEIS entry) */
    if (cache_size == 0) {
        cache[0] = 1;  /* a(0) */
        cache[1] = 1;  /* a(1) */
        cache[2] = 2;  /* a(2) */
        cache[3] = 6;  /* a(3) */
        cache[4] = 17; /* a(4) */
        cache[5] = 52; /* a(5) */
        cache[6] = 167;/* a(6) */
        cache[7] = 558;/* a(7) */
        cache[8] = 1912;/* a(8) */
        cache[9] = 6683;/* a(9) */
        cache_size = 10;
    }

    /* if requested term already computed, just return it */
    if (n < cache_size) return cache[n];

    /* compute successive terms up to n using the recurrence */
    for (idx_t k = cache_size; k <= n; ++k) {
        /* all coefficients fit in unsigned int for k ≤ 10000 */
        seq_t t1 = mul_small(cache[k-1], (unsigned int)(4*k-2));     /* (4k‑2)*a(k‑1) */
        seq_t t2 = mul_small(cache[k-3], (unsigned int)(k+1));       /* (k+1)*a(k‑3)    (negative part) */
        seq_t t3 = mul_small(cache[k-4], (unsigned int)(12*k-42));   /* (12k‑42)*a(k‑4) */
        seq_t t4 = mul_small(cache[k-7], (unsigned int)(12*k-78));   /* (12k‑78)*a(k‑7) */
        seq_t t5 = mul_small(cache[k-10], (unsigned int)(4*k-38));   /* (4k‑38)*a(k‑10) */

        /* positive part of the numerator */
        seq_t pos_sum = t1 + t3 + t4 + t5;
        /* negative part of the numerator (subtracted) */
        seq_t neg_sum = t2;

        /* numerator = pos_sum – neg_sum (guaranteed non‑negative) */
        seq_t numerator = pos_sum - neg_sum;
        /* divide by (k+1) – exact division guaranteed by the recurrence */
        cache[k] = numerator / ((seq_t)(k+1));
    }

    cache_size = n + 1;
    return cache[n];
}
