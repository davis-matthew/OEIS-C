/*
 * A312105  Coordination sequence Gal.4.87.1 where Gal.u.t.v denotes the
 *          coordination sequence for a vertex of type v in tiling number t in
 *          the Galebach list of u-uniform tilings.
 * https://oeis.org/A312105
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 49, u16 n <= 49, u32 n <= 49, u64 n <= 49, u128 n <= 49,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* OEIS A312105: Coordination Sequence Gal.4.87.1
 *
 * Recurrence:  a(n) = 2*a(n-7) - a(n-14)   for n > 16
 * Initial values for 0 ≤ n ≤ 16:
 * a(0)=1, a(1)=4, a(2)=8, a(3)=13, a(4)=18, a(5)=22, a(6)=27,
 * a(7)=33, a(8)=36, a(9)=40, a(10)=44, a(11)=49, a(12)=53,
 * a(13)=58, a(14)=67, a(15)=67, a(16)=71.
 *
 * The function stores computed values in static arrays so that
 * calls may be made in any order and by any index.  For indices
 * larger than the local cache it uses a circular buffer to avoid
 * allocating large amounts of memory.
 */

#include <stddef.h>

/* Initial values for n = 0 .. 16 */
static const seq_t init_vals[17] = {
    1, 4, 8, 13, 18, 22, 27, 33, 36, 40, 44, 49, 53, 58, 67, 67, 71
};

/* Cache for n ≤ 10000 (enough for all tests that use small n). */
#define CACHE_LIMIT 10000
static seq_t cache[CACHE_LIMIT + 1];
static size_t cache_max = 16;   /* largest n for which cache[n] is valid *//* ex: 16 */

/* Initialise the cache with the first 17 values */
static void init_cache(void)
{
    for (size_t i = 0; i <= cache_max; ++i) {
        cache[i] = init_vals[i];
    }
}

/* Compute a(n) for n up to CACHE_LIMIT incrementally and return it */
static seq_t compute_small(idx_t n)
{
    if (n <= cache_max) {
        return cache[n];
    }
    for (size_t i = cache_max + 1; i <= (size_t)n; ++i) {
        cache[i] = cache[i - 7] * ((seq_t)2) - cache[i - 14];
    }
    cache_max = (size_t)n;
    return cache[n];
}

/* Compute a(n) for n > CACHE_LIMIT using a 14‑element circular buffer */
static seq_t compute_large(idx_t n)
{
    seq_t buffer[14];
    /* initialise buffer with the first 14 terms (indices 0 .. 13) */
    for (size_t i = 0; i < 14; ++i) {
        buffer[i] = init_vals[i];
    }
    /* we already have a(0) .. a(13); start computing from a(14) */
    seq_t term = 0;
    if (n < 14) {
        return init_vals[(size_t)n];
    }
    for (size_t i = 14; i <= (size_t)n; ++i) {
        /* indices i-7 and i-14 are non‑negative because i ≥ 14 */
        size_t idx1 = (i - 7) % 14;
        size_t idx2 = (i - 14) % 14;
        term = buffer[idx1] * ((seq_t)2) - buffer[idx2];
        buffer[i % 14] = term;
    }
    return term;
}

/* Public entry point */
seq_t a(idx_t n)
{
    /* Initialise cache only once */
    if (cache_max == (size_t)16) {
        init_cache();
    }

    if (n <= (idx_t)16) {
        return init_vals[(size_t)n];
    }
    if (n <= (idx_t)CACHE_LIMIT) {
        return compute_small(n);
    }
    return compute_large(n);
}
