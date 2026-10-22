/*
 * A306350  Number of paraphyletic coalescence sequences for 2n lineages, n
 *          each in 2 species.
 * https://oeis.org/A306350
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 3*(n!)^2*(2*n-2)!*(n-1)/((n+1)*2^(2*n-3))
 * Range:     u8 n <= 2, u16 n <= 4, u32 n <= 5, u64 n <= 8, u128 n <= 13,
 *            big any n
 * Helpers:   seq_gcd
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include "../utils/seq_gcd.h"

static seq_t *cache = NULL;
static idx_t   last_n = 0;
static size_t  cache_cap = 0;

static void ensure_capacity(idx_t n)
{
    if (cache == NULL || cache_cap <= n) {
        size_t new_cap = (size_t)n + 5;
        seq_t *new_cache = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        cache = new_cache;
        cache_cap = new_cap;
    }
}

seq_t a(idx_t n)
{
    if (n < 1) return 0;

    /* Initialise base cases on first call */
    if (cache == NULL) {
        ensure_capacity(2);
        cache[1] = 0;
        cache[2] = 4;
        last_n = 2;
    }

    if (n <= last_n) return cache[n];

    if (cache_cap <= n) ensure_capacity(n);

    for (idx_t k = last_n + 1; k <= n; ++k) {
        seq_t a_prev = cache[k-1];

        /* denominator: 2*(k-2)*(k+1) */
        seq_t den = (seq_t)(2 * (k-2) * (k+1));

        /* numerator: (k-1)^2 * k^3 * (2*k-3) */
        seq_t t1 = (seq_t)(k-1);
        seq_t t2 = (seq_t)(k);
        seq_t t3 = (seq_t)(2 * k - 3);
        seq_t num = t1 * t1;                    /* (k-1)^2 */
        seq_t k3   = t2 * t2 * t2;               /* k^3 */
        num *= k3;
        num *= t3;                               /* * (2k-3) */

        /* Reduce with gcd to avoid overflow */
        seq_t g1 = seq_gcd(a_prev, den);
        a_prev /= g1;
        den     /= g1;

        seq_t g2 = seq_gcd(num, den);
        num     /= g2;
        den     /= g2;

        seq_t res = a_prev * num;
        if (den != 1) res /= den;

        cache[k] = res;
    }

    last_n = n;
    return cache[n];
}
