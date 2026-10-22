/*
 * A367920  Expansion of e.g.f. exp(4*(exp(x) - 1) - 2*x).
 * https://oeis.org/A367920
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(0) = 1; a(n) = -2*a(n-1) + 4*Σ_{j=0}^{n-1} C(n-1,j)*a(j) (n≥1)
 *            Binomial C(n,k) is supplied by the utils library. This
 *            recurrence follows directly from the definition of A367920. The
 *            function memoizes the values in a static cache so that calls
 *            with arbitrary n return immediately after initial gr...
 * Range:     u8 n <= 4, u16 n <= 7, u32 n <= 12, u64 n <= 20, u128 n <= 22,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stddef.h>
#include <stdint.h>

/* Forward declaration of the binomial helper supplied by the harness. */
seq_t binomial(idx_t n, idx_t k);

/* Static cache for previously computed terms. */
static seq_t *cache  = NULL;      /* cache[0 .. cache_size-1] holds a(i) */
static size_t cache_size = 0;
static size_t cache_cap  = 0;

/* Grow the cache so that it can hold indices up to 'need' (inclusive). */
static void
ensure_capacity(idx_t need)
{
    /* Ensure we have room for indices 0 .. need. */
    if ((size_t)need + 1 <= cache_cap) return;

    size_t new_cap = cache_cap ? cache_cap << 1 : 1;
    while (new_cap <= (size_t)need) new_cap <<= 1;

    seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
    if (!tmp) abort();            /* unrecoverable allocation failure */
    cache = tmp;
    cache_cap = new_cap;
}

/* Compute a(n) for the OEIS sequence A367920. */
seq_t
a(idx_t n)
{
    /* Compute terms up to n if needed. */
    if (cache_size <= (size_t)n) {
        ensure_capacity(n);
        for (idx_t i = cache_size; i <= n; ++i) {
            if (i == 0) {                /* a(0) = 1 */
                cache[0] = (seq_t)1;
                continue;
            }
            idx_t m = i - 1;             /* n-1 for the binomial sum */
            seq_t sum = (seq_t)0;        /* Σ C(m,j)*a(j) */
            seq_t comb = (seq_t)1;       /* C(m,0) */

            for (idx_t j = 0; j <= m; ++j) {
                /* sum += C(m,j) * a(j) */
                sum += comb * cache[j];

                /* Prepare next binomial coefficient C(m,j+1) */
                if (j < m) {
                    comb = comb * (seq_t)(m - j);
                    comb = comb / (seq_t)(j + 1);
                }
            }

            /* a(i) = -2*a(i-1) + 4*sum */
            cache[i] = sum * (seq_t)4 - cache[i - 1] * (seq_t)2;
        }
        cache_size = (size_t)n + 1;
    }
    return cache[n];
}
