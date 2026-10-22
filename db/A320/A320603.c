/*
 * A320603  a(0) = 1; if n is odd, a(n) = Product_{i=0..n-1} a(i); if n is
 *          even, a(n) = Sum_{i=0..n-1} a(i).
 * https://oeis.org/A320603
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 6, u16 n <= 8, u32 n <= 8, u64 n <= 10, u128 n <= 10,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Recurrence:
   a(0)=1, a(1)=1, a(2)=2.
   For odd n>1:  a(n)=a(n-1)*a(n-2)^2
   For even n>2: a(n)=a(n-1)+2*a(n-2)
   (indices start at 0)
*/
#include <stdlib.h>

static seq_t *cache = NULL;
static size_t cache_len = 0;

static void ensure_cache(idx_t n)
{
    size_t new_len = (size_t)n + 1;
    if (new_len <= cache_len)
        return; /* already cached */

    /* Reallocate storage */
    seq_t *new_cache = (seq_t *)realloc(cache, new_len * sizeof(seq_t));
    if (!new_cache)
        abort(); /* out of memory */
    cache = new_cache;

    /* Initialize base values if they did not exist */
    if (new_len > 0 && cache_len == 0)
        cache[0] = (seq_t)1;
    if (new_len > 1 && cache_len <= 1)
        cache[1] = (seq_t)1;
    if (new_len > 2 && cache_len <= 2)
        cache[2] = (seq_t)2;

    /* Compute new terms incrementally */
    idx_t start = (cache_len >= 3) ? (idx_t)cache_len : (idx_t)3;
    for (idx_t i = start; i <= n; ++i) {
        if ((i & 1) == 0) {               /* even */
            cache[i] = cache[i - 1] + (seq_t)2 * cache[i - 2];
        } else {                          /* odd */
            seq_t sq = cache[i - 2] * cache[i - 2];
            cache[i] = cache[i - 1] * sq;
        }
    }

    cache_len = new_len;
}

seq_t a(idx_t n)
{
    ensure_cache(n);
    return cache[n];
}
