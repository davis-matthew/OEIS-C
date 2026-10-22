/*
 * A343975  a(0) = 1; a(n) = 3 * Sum_{k=1..n} binomial(n,k) * a(k-1).
 * https://oeis.org/A343975
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 3 * ∑_{k=1..n} binomial(n,k) * a(k-1), a(0)=1
 * Range:     u8 n <= 3, u16 n <= 6, u32 n <= 11, u64 n <= 20, u128 n <= 22,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

/* cache of previously computed terms */
static seq_t *cache = NULL;   /* array of a(k) values */
static size_t cap = 0;        /* allocated capacity (in elements) */
static idx_t len = 0;         /* number of terms stored (next free index) */

seq_t a(idx_t n)
{
    /* initialise cache on first call */
    if (!cache) {
        cap = 1;
        cache = (seq_t *)malloc(cap * sizeof(seq_t));
        if (!cache) abort();
    }

    /* ensure a(0) is stored */
    if (len == 0) {
        cache[0] = (seq_t)1;
        len = 1;
    }

    if (n == 0) return cache[0];

    /* grow cache if necessary */
    if ((size_t)cap <= (size_t)n) {
        size_t new_cap = cap ? cap << 1 : 1;
        while (new_cap <= (size_t)n) {
            new_cap <<= 1;
        }
        seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        if (!tmp) abort();
        cache = tmp;
        cap = new_cap;
    }

    /* compute terms from len up to n */
    for (idx_t i = len; i <= n; ++i) {
        seq_t sum   = (seq_t)0;
        seq_t binom = (seq_t)1;            /* C(i,0) = 1 */
        for (idx_t k = 1; k <= i; ++k) {
            binom = (binom * (seq_t)(i - k + 1)) / (seq_t)k; /* C(i,k) */
            sum += binom * cache[(size_t)(k - 1)];
        }
        cache[(size_t)i] = (seq_t)3 * sum;
    }
    len = n + 1;
    return cache[(size_t)n];
}
