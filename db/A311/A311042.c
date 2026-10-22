/*
 * A311042  Coordination sequence Gal.4.11.4 where Gal.u.t.v denotes the
 *          coordination sequence for a vertex of type v in tiling number t in
 *          the Galebach list of u-uniform tilings.
 * https://oeis.org/A311042
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = a(n-1) - a(n-2) + a(n-3) - a(n-4) + a(n-5) + a(n-6) -
 *            a(n-7) + a(n-8) - a(n-9) + a(n-10) - a(n-11) initial values:
 *            a(0)=1, a(1)=4, a(2)=8, a(3)=11, a(4)=14, a(5)=19, a(6)=23,
 *            a(7)=27, a(8)=31, a(9)=33, a(10)=36, a(11)=42
 * Range:     u8 n <= 49, u16 n <= 49, u32 n <= 49, u64 n <= 49, u128 n <= 49,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>

seq_t a(idx_t n)
{
    static seq_t *cache = NULL;
    static idx_t cache_size = 0;      /* number of allocated slots */
    static idx_t max_computed = 0;    /* first index not yet computed */
    static const seq_t init[12] = {
        1, 4, 8, 11, 14, 19, 23, 27, 31, 33, 36, 42
    };

    /* Allocate or grow the cache so that index n is reachable. */
    if (cache == NULL) {
        idx_t needed = n + 1;
        if (needed < 12) needed = 12;
        cache = (seq_t *)malloc(sizeof(seq_t) * needed);
        if (!cache) return 0;      /* out of memory – unlikely in tests */

        /* initialise the first 12 terms */
        for (idx_t i = 0; i < 12 && i < needed; ++i) cache[i] = init[i];
        cache_size   = needed;
        max_computed = (cache_size >= 12 ? 12 : cache_size);
    }
    else if (cache_size <= n) {
        idx_t needed = n + 1;
        seq_t *tmp = (seq_t *)realloc(cache, sizeof(seq_t) * needed);
        if (!tmp) return 0;        /* out of memory – unlikely in tests */
        cache = tmp;
        cache_size = needed;
        /* the first 12 terms are already present from the previous allocation */
    }

    /* Return directly if the value has already been computed. */
    if (n < max_computed) return cache[n];

    /* Compute values up to n using the recurrence. */
    for (idx_t i = max_computed; i <= n; ++i) {
        __int128 val = 0;
        val += (__int128)cache[i - 1];
        val -= (__int128)cache[i - 2];
        val += (__int128)cache[i - 3];
        val -= (__int128)cache[i - 4];
        val += (__int128)cache[i - 5];
        val += (__int128)cache[i - 6];
        val -= (__int128)cache[i - 7];
        val += (__int128)cache[i - 8];
        val -= (__int128)cache[i - 9];
        val += (__int128)cache[i -10];
        val -= (__int128)cache[i -11];
        cache[i] = (seq_t)val;
    }
    max_computed = n + 1;
    return cache[n];
}
