/*
 * A367693  G.f. A(x) satisfies A(x) = 1 / ((1 + x) * (1 - x * A(x^3))).
 * https://oeis.org/A367693
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = (-1)^n + Sum_{k=0..⌊(n-1)/3⌋} a(k)*a(n-1-3*k), a(0)=1
 * Range:     u8 n <= 28, u16 n <= 50, u32 n <= 93, u64 n <= 180,
 *            u128 n <= 355, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>   /* realloc, abort */

static seq_t *seq_cache = NULL;
static idx_t seq_cache_cap = 0;
static idx_t seq_cache_len = 0;

/* Grow the cache so that indices 0..n are available. */
static void ensure_sequence_cache(idx_t n)
{
    if (n >= seq_cache_cap) {
        idx_t new_cap = seq_cache_cap ? seq_cache_cap * 2 : 16;
        while (new_cap <= n) new_cap *= 2;
        seq_t *tmp = (seq_t *)realloc(seq_cache, new_cap * sizeof(seq_t));
        if (!tmp) abort();          /* fatal error if memory allocation fails */
        seq_cache = tmp;
        seq_cache_cap = new_cap;
    }
}

seq_t a(idx_t n)
{
    /* Make sure the cache can hold up to index n */
    ensure_sequence_cache(n);

    /* Compute any missing terms up to n in increasing order */
    for (idx_t i = seq_cache_len; i <= n; ++i) {
        if (i == 0) {
            seq_cache[0] = 1;
        } else {
            seq_t sum = 0;
            idx_t maxk = (i - 1) / 3;
            for (idx_t k = 0; k <= maxk; ++k) {
                idx_t j = i - 1 - 3 * k;
                sum = sum + seq_cache[k] * seq_cache[j];
            }
            /* add (-1)^i */
            if ((i & 1) == 0)
                sum = sum + 1;    /* even i: +1 */
            else
                sum = sum - 1;    /* odd  i: -1 */
            seq_cache[i] = sum;
        }
    }

    seq_cache_len = n + 1;
    return seq_cache[n];
}
