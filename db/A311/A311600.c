/*
 * A311600  Coordination sequence Gal.4.65.1 where Gal.u.t.v denotes the
 *          coordination sequence for a vertex of type v in tiling number t in
 *          the Galebach list of u-uniform tilings.
 * https://oeis.org/A311600
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 2*a(n-7) - a(n-14) for n > 16, initial values a(0)=1,
 *            a(1)=4, a(2)=8, a(3)=12, a(4)=18, a(5)=22, a(6)=26, a(7)=32,
 *            a(8)=36, a(9)=38, a(10)=44, a(11)=50, a(12)=50, a(13)=58,
 *            a(14)=64, a(15)=68, a(16)=66
 * Range:     u8 n <= 49, u16 n <= 49, u32 n <= 49, u64 n <= 49, u128 n <= 49,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <string.h>

/* Initial values for indices 0..16 */
static const seq_t init_vals[17] = {
    1, 4, 8, 12, 18, 22, 26, 32, 36, 38,
    44, 50, 50, 58, 64, 68, 66
};

/* Cached sequence values */
static seq_t *cache = NULL;
/* Current allocation size (number of elements that can be stored) */
static size_t cap = 0;
/* Number of sequence values that have been computed so far */
static idx_t computed = 0;

/* Ensure the cached array can hold indices up to n. */
static void grow_capacity(idx_t n)
{
    if (cap <= n) {
        size_t new_cap = cap ? cap << 1 : 17;           /* double or start at 17 */
        if (new_cap <= (size_t)n) new_cap = (size_t)n + 1;
        seq_t *newp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        if (!newp) abort();  /* unrecoverable allocation failure */
        cache = newp;
        cap = new_cap;
    }
}

/* Compute a(n) using the recurrence and memoisation. */
seq_t a(idx_t n)
{
    /* Initialise the cache on the first call. */
    if (!cache) {
        cap = 17;
        cache = (seq_t *)malloc(cap * sizeof(seq_t));
        /* copy the 17 initial values */
        for (idx_t i = 0; i <= 16; ++i)
            cache[i] = init_vals[i];
        computed = 17;          /* up to index 16 inclusive */
    }

    /* If the requested index has not yet been computed, extend the cache. */
    if (n + 1 > computed) {
        grow_capacity(n);

        /* Compute values from the first missing index up to n. */
        for (idx_t i = computed; i <= n; ++i) {
            /* For i ≥ 17 the recurrence applies. */
            seq_t tmp = cache[i - 7];
            tmp += tmp;                    /* 2 * a(i-7) */
            tmp -= cache[i - 14];           /* - a(i-14) */
            cache[i] = tmp;
        }
        computed = n + 1;                  /* all indices up to n are ready */
    }

    return cache[n];
}
