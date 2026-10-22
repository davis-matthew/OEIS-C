/*
 * A345138  a(1) = 1, a(2) = 0; a(n+2) = Sum_{d|n, d < n} a(d).
 * https://oeis.org/A345138
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(1)=1, a(2)=0; a(n) = Sum_{d|n-2, d<n-2} a(d) (i.e. a(n+2) =
 *            Σ_{d|n, d<n} a(d))
 * Range:     u8 n <= 95, u16 n <= 95, u32 n <= 95, u64 n <= 95, u128 n <= 95,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

static seq_t *seq_cache = NULL;        /* 1‑based array of computed values */
static size_t seq_size = 0;           /* sizeof(seq_cache)  (>= n+1) */
static idx_t max_computed = 0;        /* largest index already filled */

/* Ensure that seq_cache is large enough to hold indices up to n (inclusive) */
static void ensure_cache(idx_t n)
{
    if (seq_cache == NULL) {
        seq_size = (size_t)n + 1;
        seq_cache = (seq_t *)malloc(seq_size * sizeof(seq_t));
        for (size_t i = 0; i < seq_size; ++i)
            seq_cache[i] = (seq_t)0;
        if (seq_size > 1) seq_cache[1] = (seq_t)1;
        if (seq_size > 2) seq_cache[2] = (seq_t)0;
        max_computed = (seq_size > 2) ? 2 : (idx_t)(seq_size - 1);
    } else if (seq_size <= (size_t)n) {
        size_t new_size = (size_t)n + 1;
        seq_cache = (seq_t *)realloc(seq_cache, new_size * sizeof(seq_t));
        for (size_t i = seq_size; i < new_size; ++i)
            seq_cache[i] = (seq_t)0;
        if (new_size > 1) seq_cache[1] = (seq_t)1;
        if (new_size > 2) seq_cache[2] = (seq_t)0;
        seq_size = new_size;
    }
}

seq_t a(idx_t n)
{
    ensure_cache(n);
    if (n <= max_computed)
        return seq_cache[(size_t)n];

    for (idx_t i = max_computed + 1; i <= n; ++i) {
        if (i <= 2) {
            seq_cache[(size_t)i] = (i == 1) ? (seq_t)1 : (seq_t)0;
            continue;
        }
        idx_t m = i - 2;          /* integer to be factored */
        seq_t sum = (seq_t)0;
        for (idx_t d = 1; d * d <= m; ++d) {
            if (m % d == 0) {
                if (d < m) sum += seq_cache[(size_t)d];
                idx_t e = m / d;
                if (e != d && e < m) sum += seq_cache[(size_t)e];
            }
        }
        seq_cache[(size_t)i] = sum;
    }
    max_computed = n;
    return seq_cache[(size_t)n];
}
