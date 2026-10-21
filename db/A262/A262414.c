/*
 * A262414  Number of (n+1) X (2+1) 0..1 arrays with each row divisible by 3
 *          and column not divisible by 3, read as a binary number with top
 *          and left being the most significant bits.
 * https://oeis.org/A262414
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 3*a(n-1) + 3*a(n-2) - 9*a(n-3) * Initial values: a(1)=0,
 *            a(2)=4, a(3)=12
 * Range:     u8 n <= 5, u16 n <= 10, u32 n <= 20, u64 n <= 40, u128 n <= 81,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

 
static seq_t *cache = NULL;
static idx_t cache_size = 0;   /* number of elements allocated; indices 0..cache_size-1 */

static void
ensure_cache(idx_t n)
{
    if (n + 1 <= cache_size)
        return;                   /* already computed up to n */

    idx_t old_size = cache_size;
    idx_t new_size = n + 1;

    seq_t *new_cache = (seq_t *)realloc(cache, (size_t)new_size * sizeof(seq_t));
    if (!new_cache)
        return;                  /* allocation failed – caller can fallback to 0 */

    cache = new_cache;
    cache_size = new_size;

    /* Initialise base cases that were not yet present */
    if (old_size <= 1 && new_size > 1)     cache[1] = (seq_t)0;
    if (old_size <= 2 && new_size > 2)     cache[2] = (seq_t)4;
    if (old_size <= 3 && new_size > 3)     cache[3] = (seq_t)12;

    /* Compute missing terms via the recurrence */
    idx_t start = (old_size > 3) ? old_size : 4;
    for (idx_t i = start; i <= n; ++i) {
        seq_t val = cache[i - 1];
        val *= (seq_t)3;                       // 3*a(i-1)

        seq_t t = cache[i - 2];
        t *= (seq_t)3;                       // 3*a(i-2)
        val += t;

        t = cache[i - 3];
        t *= (seq_t)9;                       // 9*a(i-3)
        val -= t;

        cache[i] = val;
    }
}

seq_t
a(idx_t n)
{
    if (n == 0)
        return (seq_t)0;

    ensure_cache(n);
    return cache[n];
}
