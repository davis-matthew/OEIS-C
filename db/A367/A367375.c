/*
 * A367375  Expansion of the e.g.f. (exp(x) / (5 - 4*exp(x)))^(3/5).
 * https://oeis.org/A367375
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(0) = 1 a(n) = 3*a(n-1) + 4*∑_{k=1}^{n-1} binomial(n-1,k) *
 *            a(n-k) (n ≥ 1)
 * Range:     u8 n <= 3, u16 n <= 4, u32 n <= 8, u64 n <= 13, u128 n <= 17,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stddef.h>

static seq_t *seq_cache = NULL;   /* cache of computed a(i) values */
static size_t    cache_cap = 0;    /* allocated capacity in seq_cache */
static idx_t     last_computed = 0;/* highest index already computed */

/* ensure seq_cache holds indices 0..n */
static void ensure_capacity(idx_t n)
{
    if (cache_cap > (size_t)n) return;          /* already enough */
    size_t new_cap = cache_cap ? cache_cap * 2 : 1;
    while (new_cap <= (size_t)n) new_cap <<= 1;
    seq_t *tmp = (seq_t *)realloc(seq_cache, sizeof(seq_t) * new_cap);
    if (!tmp) exit(EXIT_FAILURE);               /* memory exhaustion */
    seq_cache = tmp;
    cache_cap = new_cap;
}

/* main sequence function --------------------------------------------*/
seq_t a(idx_t n)
{
    /* initialise cache on first call */
    if (!seq_cache) {
        seq_cache = (seq_t *)malloc(sizeof(seq_t));
        if (!seq_cache) exit(EXIT_FAILURE);
        seq_cache[0] = (seq_t)1;      /* a(0) = 1 */
        cache_cap    = 1;
        last_computed= 0;
    }

    if (n == 0) return seq_cache[0];

    /* extend cached values up to n */
    if (n > last_computed) {
        ensure_capacity(n);
        for (idx_t m = last_computed + 1; m <= n; ++m) {
            seq_t sum = 0;
            seq_t C   = (seq_t)1;    /* binomial(m-1,0) */
            for (idx_t k = 1; k < m; ++k) {
                /* C = C * (m-1-(k-1)) / k  ==> C = binomial(m-1,k) */
                seq_t num = (seq_t)(m - 1 - (k - 1));
                C = (C * num) / (seq_t)k;
                sum += C * seq_cache[m - k];
            }
            seq_cache[m] = (seq_t)3 * seq_cache[m - 1] + (seq_t)4 * sum;
        }
        last_computed = n;
    }

    return seq_cache[n];
}
