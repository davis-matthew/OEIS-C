/*
 * A247535  Number of length 3+3 0..n arrays with some disjoint pairs in every
 *          consecutive four terms having the same sum.
 * https://oeis.org/A247535
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 3, u16 n <= 18, u32 n <= 210, u64 n <= 210,
 *            u128 n <= 210, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*
 * OEIS A247535:
 *   a(n) = a(n-2) + 2*a(n-3) + a(n-4) - 2*a(n-5) - 2*a(n-6) - 2*a(n-7)
 *            + a(n-8) + 2*a(n-9) + a(n-10) - a(n-12)
 *   Initial values:
 *     a(1)=8, a(2)=61, a(3)=220, a(4)=561, a(5)=1124,
 *     a(6)=2009, a(7)=3220, a(8)=4901, a(9)=7016, a(10)=9737,
 *     a(11)=13000, a(12)=17025
 *
 *   The function memoises values in a static array so that any
 *   permutation of calls returns the correct result efficiently.
 */

#include <stdlib.h>

static seq_t *cache = NULL;
static size_t cache_cap = 0;
static idx_t cache_max = 0;

/* Ensure that cache can hold indices up to 'needed-1'. */
static void
grow_cache(idx_t needed)
{
    if (cache_cap >= (size_t)needed)
        return;
    size_t new_cap = cache_cap ? (cache_cap << 1) : 16;
    while (new_cap < (size_t)needed)
        new_cap <<= 1;
    seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
    if (!tmp)
        abort(); /* memory exhausted */
    cache = tmp;
    cache_cap = new_cap;
}

/* The function computing the sequence. */
seq_t a(idx_t n)
{
    /* Handle the trivial case. */
    if (n == 0)
        return 0;

    /* First invocation: create cache and load initial values. */
    if (cache == NULL) {
        grow_cache(13);      /* indices 0..12 inclusive */
        /* initial values (index 0 unused) */
        cache[0] = (seq_t)0;
        cache[1] = (seq_t)8;
        cache[2] = (seq_t)61;
        cache[3] = (seq_t)220;
        cache[4] = (seq_t)561;
        cache[5] = (seq_t)1124;
        cache[6] = (seq_t)2009;
        cache[7] = (seq_t)3220;
        cache[8] = (seq_t)4901;
        cache[9] = (seq_t)7016;
        cache[10] = (seq_t)9737;
        cache[11] = (seq_t)13000;
        cache[12] = (seq_t)17025;
        cache_max = 12;
    }

    /* If the requested n is already computed, return immediately. */
    if (n <= cache_max)
        return cache[n];

    /* Ensure buffer capacity. */
    grow_cache(n + 1);

    /* Compute missing terms up to n. */
    for (idx_t i = cache_max + 1; i <= n; ++i) {
        seq_t pos = cache[i - 2]
                  + (cache[i - 3] << 1)
                  + cache[i - 4]
                  + cache[i - 8]
                  + (cache[i - 9] << 1)
                  + cache[i - 10];

        seq_t neg = (cache[i - 5] << 1)
                  + (cache[i - 6] << 1)
                  + (cache[i - 7] << 1)
                  + cache[i - 12];

        cache[i] = pos - neg;
    }
    cache_max = n;

    return cache[n];
}
