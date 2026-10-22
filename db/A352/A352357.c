/*
 * A352357  Expansion of e.g.f. 1/(1 - Sum_{k>=1} binomial(k+2,3) * x^k/k!).
 * https://oeis.org/A352357
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Sum_{k=1..n} binomial(k+2,3) * binomial(n,k) * a(n-k)
 *            a(0) = 1
 * Range:     u8 n <= 3, u16 n <= 6, u32 n <= 9, u64 n <= 16, u128 n <= 19,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

static seq_t *seq_cache = NULL;
static idx_t seq_cache_len = 0;

seq_t a(idx_t n)
{
    if (n < seq_cache_len)
        return seq_cache[n];

    /* Grow the cache to accommodate index n */
    size_t new_size = (size_t)n + 1;
    seq_t *new_cache = (seq_t *)realloc(seq_cache, new_size * sizeof(seq_t));
    if (!new_cache)                 /* allocation failed */
        return 0;
    seq_cache = new_cache;

    /* Initialise base case if cache is empty */
    if (seq_cache_len == 0) {
        seq_cache[0] = 1;
        seq_cache_len = 1;
        if (n == 0)
            return seq_cache[0];
    }

    /* Compute missing entries up to n */
    for (idx_t i = seq_cache_len; i <= n; ++i) {
        seq_t res = 0;
        seq_t C   = 1;
        for (idx_t k = 1; k <= i; ++k) {
            C = C * (seq_t)(i - k + 1) / (seq_t)k;
            seq_t b = (seq_t)(k + 2) * (seq_t)(k + 1) * (seq_t)k / 6;
            res += b * C * seq_cache[i - k];
        }
        seq_cache[i] = res;
    }

    seq_cache_len = n + 1;
    return seq_cache[n];
}
