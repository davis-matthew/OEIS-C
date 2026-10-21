/*
 * A255988  Number of length n+4 0..1 arrays with at most one downstep in
 *          every 4 consecutive neighbor pairs.
 * https://oeis.org/A255988
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 2*a(n-1) - a(n-2) + 3*a(n-4) - 2*a(n-5) (n ≥ 6) a(1) =
 *            26, a(2) = 45, a(3) = 80, a(4) = 144, a(5) = 256
 * Range:     u8 n <= 4, u16 n <= 14, u32 n <= 34, u64 n <= 72, u128 n <= 150,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

static seq_t *cache = NULL;   /* 1‑based cache, index 0 unused */
static idx_t  cache_size = 0; /* largest index currently stored */

/* Ensure cache can hold indices up to n_needed.  Initialise the fixed
   base values (they are the same for every expansion). */
static void ensure_cache(idx_t n_needed)
{
    if (cache_size >= n_needed)
        return;

    seq_t *tmp = (seq_t *)realloc(cache, (n_needed + 1) * sizeof(seq_t));
    if (!tmp)
        abort();                     /* allocation failed */

    cache = tmp;

    if (n_needed >= 1) cache[1] = (seq_t)26;
    if (n_needed >= 2) cache[2] = (seq_t)45;
    if (n_needed >= 3) cache[3] = (seq_t)80;
    if (n_needed >= 4) cache[4] = (seq_t)144;
    if (n_needed >= 5) cache[5] = (seq_t)256;

    cache_size = n_needed;
}

seq_t a(idx_t n)
{
    if (n == 0)
        return (seq_t)0;                 /* undefined for n < 1, return 0 */

    idx_t prev_size = cache_size;        /* value before expansion */
    ensure_cache(n);

    if (n > prev_size && n >= 6) {
        for (idx_t i = prev_size + 1; i <= n; ++i) {
            if (i < 6)          /* base values already set */
                continue;
            seq_t val = (seq_t)2 * cache[i-1];
            val -= cache[i-2];
            val += (seq_t)3 * cache[i-4];
            val -= (seq_t)2 * cache[i-5];
            cache[i] = val;
        }
    }

    cache_size = n;                     /* we now have all values up to n */
    return cache[n];
}
