/*
 * A369794  Expansion of 1/(1 - x^5/(1-x)^6).
 * https://oeis.org/A369794
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 6*a(n-1) - 15*a(n-2) + 20*a(n-3) - 15*a(n-4) + 7*a(n-5) -
 *            a(n-6) for n > 6, with a(0)=1, a(1)=a(2)=a(3)=a(4)=0, a(5)=1,
 *            a(6)=6
 * Range:     u8 n <= 10, u16 n <= 17, u32 n <= 32, u64 n <= 34, u128 n <= 34,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdint.h>
#include <stdlib.h>


static seq_t *cache = NULL;      /* memoised values */
static idx_t cache_cap = 0;      /* current allocated capacity (>= cache_len+1) */
static idx_t cache_len = 0;      /* largest index for which a(i) is stored */

/* Ensure that indices 0..n can be stored.  Newly allocated slots are zeroed. */
static void ensure_capacity(idx_t n)
{
    if (n < cache_cap) return;
    idx_t new_cap = cache_cap ? cache_cap * 2 : 16;
    while (new_cap <= n) new_cap <<= 1;
    seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
    if (!tmp && new_cap > 0) abort();   /* allocation failure */
    cache = tmp;
    /* zero the newly allocated portion */
    for (idx_t i = cache_cap; i < new_cap; ++i) cache[i] = (seq_t)0;
    cache_cap = new_cap;
}

/* One-time cache initialisation with the known first values. */
static void init_cache(void)
{
    if (cache) return;
    cache_cap = 16;
    cache = (seq_t *)calloc(cache_cap, sizeof(seq_t));
    if (!cache) abort();
    cache[0] = (seq_t)1;   /* a(0) */
    cache[5] = (seq_t)1;   /* a(5) */
    cache[6] = (seq_t)6;   /* a(6) */
    /* indices 1-4 and any future slots are already zeroed */
    cache_len = 6;
}

/* Main sequence function. */
seq_t a(idx_t n)
{
    if (!cache) init_cache();

    if (n <= cache_len) return cache[n];

    ensure_capacity(n);

    for (idx_t i = cache_len + 1; i <= n; ++i)
    {
        if (i <= 6)
            ;  /* values 1-6 are already set (zero or predefined) */
        else
            cache[i] = 6      * cache[i-1]
                     - 15     * cache[i-2]
                     + 20     * cache[i-3]
                     - 15     * cache[i-4]
                     +  7     * cache[i-5]
                     -  1     * cache[i-6];
    }

    cache_len = n;
    return cache[n];
}
