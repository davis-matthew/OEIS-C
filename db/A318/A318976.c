/*
 * A318976  Expansion of e.g.f. Product_{k>=1} ((1 + x^k)/(1 -
 *          x^k))^(phi(k)/k), where phi is the Euler totient function A000010.
 * https://oeis.org/A318976
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = n! · [xⁿ] exp(x (2+x)/(1−x²)) Recurrence (derived from
 *            A’(x)=f’(x)A(x)): a[0] = 1 for n≥0: a[n+1] = Σ_{k=0}^{n} t_k ·
 *            P(n,k) · a[n–k] where t_k = (k+1) · c_{k+1}, c_{k+1} = 2 if k is
 *            even (k+1 odd), otherwise 1. P(n,k) = n·(n−1)·…·(n−k+1) (falling
 *            factorial)
 * Range:     u8 n <= 4, u16 n <= 6, u32 n <= 11, u64 n <= 18, u128 n <= 31,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    /* static cache for memoisation */
    static seq_t *cache = NULL;   /* array of a[i] values   */
    static size_t cap   = 0;      /* allocated array size   */
    static size_t len   = 0;      /* number of computed terms */

    /* initialise cache with a[0] */
    if (!cache) {
        cap = 1;
        cache = (seq_t *)malloc(sizeof(seq_t));
        if (!cache) return 0;           /* allocation failure */
        cache[0] = (seq_t)1;
        len   = 1;
    }

    /* grow the cache to accommodate index n */
    if ((size_t)n >= cap) {
        size_t newcap = cap == 0 ? 1 : cap;
        while (newcap <= (size_t)n)
            newcap <<= 1;
        seq_t *tmp = (seq_t *)realloc(cache, newcap * sizeof(seq_t));
        if (!tmp) return 0;             /* realloc failure */
        cache = tmp;
        cap   = newcap;
    }

    /* compute missing values up to n */
    for (size_t cur = len; cur <= (size_t)n; ++cur) {
        seq_t sum = (seq_t)0;
        seq_t P   = (seq_t)1;          /* P(cur-1,0)   */
        idx_t   m  = cur - 1;           /* upper bound for k */

        /* Σ_{k=0}^{m} t_k · P(m,k) · a[m-k]   */
        for (idx_t k = 0; k <= m; ++k) {
            seq_t t;
            if ((k & 1) == 0)          /* k even -> c = 2   */
                t = ((seq_t)(k + 1)) * (seq_t)2;
            else                       /* k odd  -> c = 1   */
                t = ((seq_t)(k + 1)) * (seq_t)1;

            sum += t * P * cache[(size_t)(cur - 1 - k)];

            if (k < m)
                P *= (seq_t)(m - k);    /* update to P(m,k+1) */
        }

        cache[(size_t)cur] = sum;
    }

    len = (size_t)n + 1;
    return cache[(size_t)n];
}
