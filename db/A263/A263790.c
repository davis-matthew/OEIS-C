/*
 * A263790  The number of length-n permutations avoiding the patterns 1234,
 *          1324 and 2143.
 * https://oeis.org/A263790
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(0)=1, a(1)=1, a(2)=2, a(3)=6
 *            a(n)=4*a(n-1)-2*a(n-2)+2*a(n-3)-a(n-4) for n>=4 (recurrence
 *            derived from the rational generating function)
 * Range:     u8 n <= 5, u16 n <= 10, u32 n <= 19, u64 n <= 28, u128 n <= 28,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

static seq_t *cache = NULL;
static size_t cache_cap = 0;

static void ensure_cache(idx_t n)
{
    if (n < cache_cap) return;
    size_t new_cap = (size_t)n + 1;
    seq_t *new_cache = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
    if (new_cache == NULL) {
        /* allocation failed – terminate execution */
        abort();
    }
    cache = new_cache;
    for (size_t k = cache_cap; k <= n; ++k) {
        if (k == 0) cache[k] = (seq_t)1;
        else if (k == 1) cache[k] = (seq_t)1;
        else if (k == 2) cache[k] = (seq_t)2;
        else if (k == 3) cache[k] = (seq_t)6;
        else {
            /* recurrence: a[n] = 4*a[n-1] - 2*a[n-2] + 2*a[n-3] - a[n-4] */
            seq_t val = cache[k-1] * (seq_t)4;
            val -= cache[k-2] * (seq_t)2;
            val += cache[k-3] * (seq_t)2;
            val -= cache[k-4];
            cache[k] = val;
        }
    }
    cache_cap = new_cap;
}

seq_t a(idx_t n)
{
    ensure_cache(n);
    return cache[n];
}
