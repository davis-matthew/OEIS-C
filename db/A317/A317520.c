/*
 * A317520  Number of nX3 0..1 arrays with every element unequal to 0, 1, 2,
 *          3, 4, 5, 6 or 8 king-move adjacent elements, with upper left
 *          element zero.
 * https://oeis.org/A317520
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 8*a(n-1) - 2*a(n-2) + a(n-3) - 3*a(n-4) Initial values:
 *            a(1)=4, a(2)=32, a(3)=248, a(4)=1921. The sequence is indexed
 *            from 1
 * Range:     u8 n <= 3, u16 n <= 5, u32 n <= 11, u64 n <= 21, u128 n <= 43,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

static seq_t *cache = NULL;   /* array of computed terms, 1‑based */
static idx_t cached_max = 0;  /* largest n currently stored */

/* Ensure that cache contains values up to n (inclusive). */
static void ensure_cache(idx_t n)
{
    /* nothing to do if we already have enough */
    if (n <= cached_max)
        return;

    /* resize cache array to hold indices 0..n */
    seq_t *new_cache = (seq_t *)realloc(cache, sizeof(seq_t) * (n + 1));
    if (!new_cache)
        abort(); /* cannot allocate more memory */

    cache = new_cache;

    for (idx_t i = cached_max + 1; i <= n; ++i)
    {
        if (i == 1)
            cache[i] = (seq_t)4;
        else if (i == 2)
            cache[i] = (seq_t)32;
        else if (i == 3)
            cache[i] = (seq_t)248;
        else if (i == 4)
            cache[i] = (seq_t)1921;
        else
            cache[i] = (seq_t)8    * cache[i - 1]
                      - (seq_t)2  * cache[i - 2]
                      +          cache[i - 3]
                      - (seq_t)3  * cache[i - 4];
    }

    cached_max = n;
}

seq_t a(idx_t n)
{
    if (n < 1)
        return 0;          /* undefined for 0, but safety for caller */
    ensure_cache(n);
    return cache[n];
}
