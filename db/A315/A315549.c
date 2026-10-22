/*
 * A315549  Coordination sequence Gal.6.628.2 where Gal.u.t.v denotes the
 *          coordination sequence for a vertex of type v in tiling number t in
 *          the Galebach list of u-uniform tilings.
 * https://oeis.org/A315549
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = a(n-1) + a(n-3) - a(n-4) - a(n-6) + a(n-7) + a(n-9) -
 *            a(n-10) for n > 10. Initial terms: a(0)=1, a(1)=6, a(2)=11,
 *            a(3)=17, a(4)=23, a(5)=29, a(6)=35, a(7)=41, a(8)=47, a(9)=53,
 *            a(10)=59
 * Range:     u8 n <= 43, u16 n <= 49, u32 n <= 49, u64 n <= 49, u128 n <= 49,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

/* Cache for already computed values */
static seq_t *cache = NULL;          /* pointer to cached elements */
static idx_t  cache_len = 0;        /* number of elements stored (always > 0 after init) */
static idx_t  cache_cap = 0;        /* capacity of the cache array */

/* Ensure that cache has capacity for indices up to n (inclusive) */
static void ensure_capacity(idx_t n)
{
    if (cache_cap > n) return;
    /* Expand capacity: double until we can hold n+1 elements */
    idx_t new_cap = cache_cap ? cache_cap * 2 : 16;
    while (new_cap <= n) new_cap *= 2;
    seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
    if (!tmp) exit(1);  /* memory allocation failed */
    cache = tmp;
    cache_cap = new_cap;
    /* New slots are uninitialized; they will be filled when computed. */
}

seq_t a(idx_t n)
{
    /* Lazy initialization of the cache with the first 11 terms */
    if (!cache) {
        cache_cap = 16;
        cache = (seq_t *)calloc(cache_cap, sizeof(seq_t));
        if (!cache) exit(1);  /* allocation failure */
        /* initial terms */
        cache[0] = 1;
        cache[1] = 6;
        cache[2] = 11;
        cache[3] = 17;
        cache[4] = 23;
        cache[5] = 29;
        cache[6] = 35;
        cache[7] = 41;
        cache[8] = 47;
        cache[9] = 53;
        cache[10] = 59;
        cache_len = 11;          /* we have computed elements 0..10 */
    }

    if (n < cache_len) return cache[n];

    ensure_capacity(n);

    for (idx_t i = cache_len; i <= n; ++i) {
        /* According to the recurrence, valid for i >= 11 */
        seq_t val = cache[i-1];
        val += cache[i-3];
        val -= cache[i-4];
        val -= cache[i-6];
        val += cache[i-7];
        val += cache[i-9];
        val -= cache[i-10];
        cache[i] = val;
    }
    cache_len = n + 1;
    return cache[n];
}
