/*
 * A241155  a(n)=1 for n <= s+k; thereafter a(n) = Sum_{i=0..k-1}
 *          a(n-i-s-a(n-i-1)) where s=0, k=6.
 * https://oeis.org/A241155
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 304, u16 n <= 9227, u32 n <= 9227, u64 n <= 9227,
 *            u128 n <= 9227, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Recurrence (OEIS A241155):
   a(n) = 1                                   for n ≤ 6
   a(n) = Σ_{i=0}^{5} a( n - i - a( n - i - 1 ) )   for n > 6
   (here s = 0, k = 6)
*/

#include <stdlib.h>

static seq_t *cache = NULL;            /* cached values (index 0 unused) */
static size_t cache_cap = 0;           /* allocated length of cache array   */
static size_t max_computed = 0;        /* highest index with a value stored */

/* ensure that cache can hold indices 0 .. n (inclusive) */
static void ensure_cache(size_t n)
{
    if (cache_cap > n)
        return;

    size_t new_cap = cache_cap ? cache_cap * 2 : 8;
    while (new_cap <= n)
        new_cap *= 2;

    seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
    if (!tmp)
        abort();                      /* allocation failed */

    cache = tmp;

    if (cache_cap == 0) {
        /* initialise the static array */
        cache[0] = 0;                 /* a(0) is unused but needed for indexing */
        for (size_t i = 1; i <= 6; ++i)
            cache[i] = 1;             /* a(1)..a(6) = 1 */
        max_computed = 6;
    }

    cache_cap = new_cap;
}

seq_t a(idx_t n)
{
    if (n <= 6)
        return 1;                     /* a(1)..a(6)=1 */

    size_t m = (size_t)n;
    ensure_cache(m);

    if (m <= max_computed)           /* already computed */
        return cache[m];

    for (size_t i = max_computed + 1; i <= m; ++i) {
        seq_t sum = 0;
        for (int j = 0; j < 6; ++j) {               /* j = 0..5 */
            size_t idx = i - j - (size_t)cache[i - j - 1];
            sum += cache[idx];
        }
        cache[i] = sum;
    }

    max_computed = m;
    return cache[m];
}
