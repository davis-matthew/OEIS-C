/*
 * A313796  Coordination sequence Gal.6.364.5 where Gal.u.t.v denotes the
 *          coordination sequence for a vertex of type v in tiling number t in
 *          the Galebach list of u-uniform tilings.
 * https://oeis.org/A313796
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 2*a(n-1) - a(n-2) - a(n-6) + 2*a(n-7) - a(n-8) - a(n-12)
 *            + 2*a(n-13) - a(n-14) for n > 14 Initial values: a(0)=1, a(1)=5,
 *            a(2)=10, a(3)=15, a(4)=21, a(5)=27, a(6)=32, a(7)=38, a(8)=43,
 *            a(9)=48, a(10)=53, a(11)=58, a(12)=64, a(13)=69, a(14)=75
 * Range:     u8 n <= 47, u16 n <= 49, u32 n <= 49, u64 n <= 49, u128 n <= 49,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stddef.h>

static seq_t *cache = NULL;
static size_t cache_capacity = 0; /* number of allocated slots */
static size_t cache_size = 0;     /* number of values already computed */

/* Base values of the sequence (a(0) … a(14)). */
static const seq_t base_vals[15] = {
    (seq_t)1, (seq_t)5, (seq_t)10, (seq_t)15, (seq_t)21,
    (seq_t)27, (seq_t)32, (seq_t)38, (seq_t)43, (seq_t)48,
    (seq_t)53, (seq_t)58, (seq_t)64, (seq_t)69, (seq_t)75
};

/* Compute a(n) as defined above. */
seq_t a(idx_t n)
{
    /* Ensure we have a cache holding at least n+1 entries. */
    size_t needed = (size_t)n + 1;
    if (cache_capacity < needed) {
        size_t new_cap = cache_capacity ? cache_capacity : 16;
        while (new_cap < needed) new_cap <<= 1;
        seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        if (!tmp) abort(); /* memory allocation failure */
        cache = tmp;
        cache_capacity = new_cap;
    }

    /* First time: fill base values up to 15. */
    if (cache_size == 0) {
        for (size_t i = 0; i < 15; ++i) cache[i] = base_vals[i];
        cache_size = 15;
    }

    /* If the requested term is already computed, return it. */
    if (n < cache_size) return cache[(size_t)n];

    /* Compute terms from cache_size up to n inclusive. */
    for (size_t i = cache_size; i <= (size_t)n; ++i) {
        /* For i < 15 the value is already in cache. */
        if (i < 15) continue;

        seq_t val = 0;
        val += (seq_t)2 * cache[i - 1];
        val -= cache[i - 2];
        val -= cache[i - 6];
        val += (seq_t)2 * cache[i - 7];
        val -= cache[i - 8];
        val -= cache[i - 12];
        val += (seq_t)2 * cache[i - 13];
        val -= cache[i - 14];
        cache[i] = val;
    }

    cache_size = (size_t)n + 1;
    return cache[(size_t)n];
}
