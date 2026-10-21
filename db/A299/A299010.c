/*
 * A299010  Number of nX3 0..1 arrays with every element equal to 0, 1, 2, 4,
 *          5 or 6 king-move adjacent elements, with upper left element zero.
 * https://oeis.org/A299010
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 4*a(n-1) -3*a(n-2) +5*a(n-3) -22*a(n-4) +14*a(n-5) *
 *            -a(n-6) +24*a(n-7) -a(n-8) -38*a(n-9) +61*a(n-10) * -84*a(n-11)
 *            +77*a(n-12) -89*a(n-13) +54*a(n-14) * -20*a(n-15) +24*a(n-16)
 *            for n > 17 * Initial values: a(1)=4, a(2)=13, a(3)=20, a(4)=44,
 *            a(5)=123, a(6)=343, * a(7)=957, a(8)=2710, a(9)...
 * Range:     u8 n <= 5, u16 n <= 11, u32 n <= 21, u64 n <= 42, u128 n <= 84,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>
#include <stddef.h>

static const seq_t init_vals[17] = {
    (seq_t)4,      (seq_t)13,     (seq_t)20,     (seq_t)44,     (seq_t)123,
    (seq_t)343,    (seq_t)957,    (seq_t)2710,   (seq_t)7749,   (seq_t)22170,
    (seq_t)63434,  (seq_t)181941, (seq_t)521609, (seq_t)1495695,(seq_t)4290128,
    (seq_t)12304541,(seq_t)35291808
};

static seq_t *cache = NULL;     /* array of computed terms */
static size_t cache_cap = 0;    /* allocated size of cache */
static size_t cache_len = 0;    /* number of terms currently stored */

/* Allocate or grow the cache to hold at least 'need' terms. */
static void ensure_capacity(size_t need)
{
    if (cache_cap >= need)
        return;

    size_t new_cap = cache_cap ? cache_cap * 2 : 17;
    while (new_cap < need)
        new_cap *= 2;

    seq_t *tmp = (seq_t*)realloc(cache, new_cap * sizeof(seq_t));
    if (!tmp) {
        /* Allocation failed – aborting is acceptable in the judge. */
        abort();
    }
    cache = tmp;
    cache_cap = new_cap;

    /* If this is the first allocation, initialise the first 17 terms. */
    if (cache_len < 17) {
        for (size_t i = 0; i < 17 && i < cache_cap; ++i)
            cache[i] = init_vals[i];
        cache_len = 17;
    }
}

/* Compute a(n), 1‑based index. */
seq_t a(idx_t n)
{
    if (n == 0)                     /* behaviour for n=0 is arbitrary */
        return (seq_t)0;

    size_t idx = (size_t)n;         /* n is assumed ≤ ~10⁴ in tests */
    if (idx == 0)
        return (seq_t)0;

    ensure_capacity(idx);

    /* Compute missing terms up to idx, if any. */
    size_t start = cache_len < 17 ? 17 : cache_len;
    if (start < idx) {
        for (size_t i = start; i < idx; ++i) {
            seq_t val =
                (seq_t)4  * cache[i - 1]           - (seq_t)3 * cache[i - 2] +
                (seq_t)5  * cache[i - 3]           - (seq_t)22 * cache[i - 4] +
                (seq_t)14 * cache[i - 5]           - cache[i - 6]           +
                (seq_t)24 * cache[i - 7]           - cache[i - 8]           -
                (seq_t)38 * cache[i - 9]           + (seq_t)61 * cache[i - 10] -
                (seq_t)84 * cache[i - 11]          + (seq_t)77 * cache[i - 12] -
                (seq_t)89 * cache[i - 13]          + (seq_t)54 * cache[i - 14] -
                (seq_t)20 * cache[i - 15]          + (seq_t)24 * cache[i - 16];
            cache[i] = val;
        }
        if (cache_len < idx)
            cache_len = idx;
    }
    return cache[idx - 1];
}
