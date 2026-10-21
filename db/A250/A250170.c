/*
 * A250170  Number of length 5+1 0..n arrays with the sum of adjacent
 *          differences multiplied by some arrangement of +-1 equal to zero.
 * https://oeis.org/A250170
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   Recursive definition for n > 24 a(n) = a(n-2) + a(n-3) +
 *            2*a(n-4) - a(n-6) - 3*a(n-7) - 3*a(n-8) - a(n-9) + 3*a(n-11) +
 *            4*a(n-12) + 3*a(n-13) - a(n-15) - 3*a(n-16) - 3*a(n-17) -
 *            a(n-18) + 2*a(n-20) + a(n-21) + a(n-22) - a(n-24) Initial values
 *            (offset 1): a(1)=32, a(2)=373, a(3)=1880, a(4)=7109, a...
 * Range:     u8 n <= 1, u16 n <= 6, u32 n <= 57, u64 n <= 66, u128 n <= 66,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

/* external types defined by the build */
extern seq_t a(idx_t n);

seq_t a(idx_t n)
{
    /* a(0) is unused but returned for completeness */
    if (n == 0) return 0;

    static seq_t *cache = NULL;   /* dynamic cache of computed terms */
    static size_t cache_cap = 0;  /* allocated capacity (number of slots) */
    static size_t cache_size = 0; /* number of valid entries (index one past last filled) */

    /* initialise cache with the first 24 values on first call */
    if (cache == NULL) {
        /* allocate space for indices 0..24 (25 slots); index 0 is unused */
        cache_cap = 25;
        cache = (seq_t*)malloc(cache_cap * sizeof(seq_t));
        if (cache == NULL) return 0;     /* out of memory – give up */

        /* initial values, zero for index 0 */
        cache[0] = 0;
        cache[1] = 32;
        cache[2] = 373;
        cache[3] = 1880;
        cache[4] = 7109;
        cache[5] = 19896;
        cache[6] = 49037;
        cache[7] = 103556;
        cache[8] = 203615;
        cache[9] = 364900;
        cache[10] = 624811;
        cache[11] = 1006084;
        cache[12] = 1570791;
        cache[13] = 2347840;
        cache[14] = 3431579;
        cache[15] = 4856212;
        cache[16] = 6757417;
        cache[17] = 9171308;
        cache[18] = 12285541;
        cache[19] = 16134624;
        cache[20] = 20968689;
        cache[21] = 26816804;
        cache[22] = 34003157;
        cache[23] = 42544984;
        cache[24] = 52855367;

        cache_size = 25; /* one past the last valid index */
    }

    /* if the requested index is already cached, return it directly */
    if ((size_t)n < cache_size)
        return cache[(size_t)n];

    /* enlarge the cache if necessary (grow by factor 2 each time) */
    if ((size_t)n >= cache_cap) {
        size_t new_cap = cache_cap;
        while (new_cap <= (size_t)n) new_cap <<= 1;
        seq_t *tmp = (seq_t*)realloc(cache, new_cap * sizeof(seq_t));
        if (tmp == NULL) return 0;   /* out of memory – give up */
        cache = tmp;
        cache_cap = new_cap;
    }

    /* compute missing terms up to and including n */
    for (size_t i = cache_size; i <= (size_t)n; ++i) {
        seq_t val = 0;
        val += cache[i-2];
        val += cache[i-3];
        val += cache[i-4] * 2;
        val -= cache[i-6];
        val -= cache[i-7] * 3;
        val -= cache[i-8] * 3;
        val -= cache[i-9];
        val += cache[i-11] * 3;
        val += cache[i-12] * 4;
        val += cache[i-13] * 3;
        val -= cache[i-15];
        val -= cache[i-16] * 3;
        val -= cache[i-17] * 3;
        val -= cache[i-18];
        val += cache[i-20] * 2;
        val += cache[i-21];
        val += cache[i-22];
        val -= cache[i-24];

        cache[i] = val;
    }
    /* update the size of known terms */
    cache_size = (size_t)n + 1;
    return cache[(size_t)n];
}
