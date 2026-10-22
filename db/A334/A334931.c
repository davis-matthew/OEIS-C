/*
 * A334931  Numbers that generate rotationally symmetrical XOR-triangles with
 *          a pattern of zero-triangles of edge length 2, some of which are
 *          clipped to result in some singleton zeros at the edges.
 * https://oeis.org/A334931
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 2097153*a(n-8) - 2097152*a(n-16) (n ≥ 17) Initial values:
 *            a(1)=151, a(2)=233, a(3)=1483, a(4)=1693, a(5)=10707,
 *            a(6)=13029, a(7)=644007, a(8)=941241, a(9)=317049751,
 *            a(10)=490370281, a(11)=3111314891, a(12)=3550957213,
 *            a(13)=22455577043, a(14)=27325461221, a(15)=1350581212071,
 *            a(16)=197392...
 * Range:     u8 n <= 2, u16 n <= 6, u32 n <= 12, u64 n <= 24, u128 n <= 48,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

static seq_t *cache = NULL;
static size_t cache_cap = 0;
static size_t cache_size = 0;

static const seq_t init_vals[17] = {
    (seq_t)0,               /* index 0 unused */
    (seq_t)151, (seq_t)233, (seq_t)1483, (seq_t)1693,
    (seq_t)10707, (seq_t)13029, (seq_t)644007, (seq_t)941241,
    (seq_t)317049751, (seq_t)490370281, (seq_t)3111314891,
    (seq_t)3550957213, (seq_t)22455577043, (seq_t)27325461221,
    (seq_t)1350581212071, (seq_t)1973926386873
};

seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0;          /* sequence undefined below offset */

    /* Ensure the cache is large enough to hold all indices up to n */
    if ((size_t)n + 1 > cache_cap) {
        size_t new_cap = (size_t)n + 1;
        if (new_cap < 17) new_cap = 17;   /* at least space for the first 16 terms */
        seq_t *new_cache = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        if (!new_cache) exit(EXIT_FAILURE);
        cache = new_cache;
        cache_cap = new_cap;
        /* Zero the newly allocated part (not strictly necessary) */
        for (size_t i = cache_size + 1; i < new_cap; ++i)
            cache[i] = (seq_t)0;
    }

    /* Initialize the known first 16 terms once */
    if (cache_size < 16) {
        for (idx_t i = 1; i <= 16; ++i)
            cache[i] = init_vals[i];
        cache[0] = (seq_t)0;              /* optional padding */
        cache_size = 16;
    }

    /* Return the cached value if already computed */
    if (n <= cache_size) return cache[(size_t)n];

    /* Compute missing values up to n using the recurrence */
    for (idx_t i = cache_size + 1; i <= n; ++i) {
        seq_t a8  = cache[(size_t)(i - 8)];
        seq_t a16 = cache[(size_t)(i - 16)];
        cache[(size_t)i] = ((seq_t)2097153 * a8) - ((seq_t)2097152 * a16);
    }
    cache_size = n;

    return cache[(size_t)n];
}
