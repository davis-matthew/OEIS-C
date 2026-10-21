/*
 * A252586  Numbers n such that the heptagonal number H(n) is equal to the sum
 *          of the pentagonal numbers P(m) and P(m+1) for some m.
 * https://oeis.org/A252586
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = a(n-1) + 482*a(n-2) - 482*a(n-3) - a(n-4) + a(n-5)
 *            Initial values: a(1)=4, a(2)=257, a(3)=1772, a(4)=123729,
 *            a(5)=853956
 * Range:     u8 n <= 1, u16 n <= 3, u32 n <= 7, u64 n <= 14, u128 n <= 29,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stddef.h>

static seq_t *cache = NULL;
static idx_t cache_len = 0;   /* highest index that has been computed */
static idx_t cache_cap = 0;   /* allocated capacity (indexes 1..cache_cap) */

/* Ensure the internal cache can hold at least `needed` entries */
static void grow_cache(idx_t needed)
{
    if (needed <= cache_cap)
        return;
    idx_t new_cap = cache_cap ? cache_cap * 2 : 16;
    while (new_cap < needed)
        new_cap <<= 1;
    seq_t *new_mem = (seq_t *)realloc(cache, (new_cap + 1) * sizeof(seq_t));
    if (!new_mem) abort();        /* allocation failure */
    cache = new_mem;
    cache_cap = new_cap;
}

/* Initialize the cache with the first five terms of the sequence */
static void init_cache(void)
{
    if (cache_len != 0)
        return;                  /* already initialized */
    grow_cache(16);              /* give a little headroom */
    cache[1] = (seq_t)4;
    cache[2] = (seq_t)257;
    cache[3] = (seq_t)1772;
    cache[4] = (seq_t)123729;
    cache[5] = (seq_t)853956;
    cache_len = 5;
}

/* Return the nth term of A252586 (1‑based index) */
seq_t a(idx_t n)
{
    if (n == 0)                 /* undefined, return 0 for safety */
        return (seq_t)0;
    init_cache();

    if (n > cache_len)
    {
        grow_cache(n);
        for (idx_t i = cache_len + 1; i <= n; ++i)
        {
            seq_t t1 = cache[i - 1];
            seq_t t2 = (seq_t)482 * cache[i - 2];
            seq_t t3 = (seq_t)482 * cache[i - 3];
            seq_t t4 = cache[i - 4];
            seq_t t5 = cache[i - 5];
            cache[i] = t1 + t2 - t3 - t4 + t5;
        }
        cache_len = n;
    }
    return cache[n];
}
