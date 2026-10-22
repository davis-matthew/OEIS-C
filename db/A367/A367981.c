/*
 * A367981  Expansion of e.g.f. exp(-x) / (2 - exp(4*x)).
 * https://oeis.org/A367981
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = (-1)^n + Σ_{k=1..n} binomial(n,k)*4^k*a(n-k); a(0) = 1
 * Range:     u8 n <= 2, u16 n <= 4, u32 n <= 7, u64 n <= 12, u128 n <= 22,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

/* static cache for memoization */
static seq_t *cache = NULL;
static idx_t cache_cap = 0;   /* allocated size */
static idx_t computed_len = 0; /* number of entries already computed (up to computed_len-1) */

/* ensure the cache has capacity for indices 0..need */
static void ensure_capacity(idx_t need)
{
    if (cache_cap <= need) {
        idx_t new_cap = (need + 1) * 2;
        seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        if (!tmp) abort();          /* fatal error if allocation fails */
        cache = tmp;
        cache_cap = new_cap;
    }
}

/* compute a(n) for all indices up to n, storing results in cache */
static void compute_up_to(idx_t n)
{
    if (n < computed_len) return;        /* already computed */

    ensure_capacity(n);

    /* initialise base case if not yet done */
    if (computed_len == 0) {
        cache[0] = (seq_t)1;
        computed_len = 1;
    }

    for (idx_t m = computed_len; m <= n; ++m) {
        seq_t sum = 0;
        seq_t comb = 1;          /* C(m,0) */
        seq_t pow4 = 1;          /* 4^0 */

        for (idx_t k = 1; k <= m; ++k) {
            /* update to C(m,k) */
            comb = comb * (m - k + 1) / (seq_t)k;
            pow4 = pow4 * (seq_t)4;

            seq_t tmp = cache[m - k];
            tmp = tmp * comb;
            tmp = tmp * pow4;
            sum += tmp;
        }

        /* add (-1)^m */
        if (m & 1) {
            sum -= (seq_t)1;     /* m odd */
        } else {
            sum += (seq_t)1;     /* m even */
        }

        cache[m] = sum;
    }
    computed_len = n + 1;
}

/* public sequence function */
seq_t a(idx_t n)
{
    compute_up_to(n);
    return cache[n];
}
