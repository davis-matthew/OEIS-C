/*
 * A336427  a(0) = 0; a(n) = 1 + (1/n) * Sum_{k=1..n-1} binomial(n,k)^3 * k *
 *          a(k).
 * https://oeis.org/A336427
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 1 + (1/n) * Σ_{k=1}^{n-1} binomial(n,k)^3 * k * a(k) with
 *            a(0)=0. This implementation uses 128‑bit integers for
 *            intermediate calculations when available; otherwise it falls
 *            back to 64‑bit arithmetic. A static cache is maintained so that
 *            calls are O(n) in the largest n requested
 * Range:     u8 n <= 3, u16 n <= 4, u32 n <= 6, u64 n <= 10, u128 n <= 14,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>

#if defined(__SIZEOF_INT128__)
typedef unsigned __int128 big_t;
#else
typedef unsigned long long big_t;  /* safe for n up to 10 on 64‑bit builds */
#endif

seq_t a(idx_t n)
{
    /* Static cache for memoisation */
    static seq_t *cache = NULL;
    static size_t cap = 0;
    static idx_t max_cum = 0;  /* largest n already computed */

    /* Ensure cache capacity */
    if (n + 1 > cap) {
        size_t new_cap = (n + 1 > 16) ? n + 1 : 16;
        while (new_cap <= cap)
            new_cap <<= 1;
        seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        if (!tmp) exit(1);
        cache = tmp;
        cap   = new_cap;
    }

    /* Initialise base case in the cache */
    if (max_cum == 0) {
        cache[0] = 0;
    }

    /* Return if already computed */
    if (n <= max_cum)
        return cache[n];

    /* Compute values from (max_cum+1) up to n */
    for (idx_t val = max_cum + 1; val <= n; ++val) {
        if (val == 0) { cache[0] = 0; continue; }
        if (val == 1) { cache[1] = 1; continue; }

        big_t sum = 0;                     /* Σ term */
        big_t comb = 1;                    /* start with C(val,0)=1 */

        for (idx_t k = 1; k < val; ++k) {
            /* Update comb to C(val,k) via multiplicative formula.
               comb = comb * (val - k + 1) / k;   all in 128‑bit */
            comb = (comb * (big_t)(val - k + 1)) / (big_t)k;

            /* term = comb^3 * k * a(k) */
            big_t term = comb;
            term *= comb;        /* comb^2 */
            term *= comb;        /* comb^3 */
            term *= (big_t)k;    /* *k */
            term *= (big_t)cache[k];   /* *a(k) */

            sum += term;
        }

        big_t res = (sum / (big_t)val) + 1;   /* a(val) = 1 + sum/n */
        cache[val] = (seq_t)res;
    }

    max_cum = n;
    return cache[n];
}
