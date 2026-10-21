/*
 * A283582  Binary representation of the x-axis, from the origin to the right
 *          edge, of the n-th stage of growth of the two-dimensional cellular
 *          automaton defined by "Rule 646", based on the 5-celled von Neumann
 *          neighborhood.
 * https://oeis.org/A283582
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 100*a(n-2) + a(n-8) - 100*a(n-10) (for n > 11) Initial
 *            values: a(0)=1, a(1)=11, a(2)=101, a(3)=1011, a(4)=10101,
 *            a(5)=101011, a(6)=1000101, a(7)=11011011, a(8)=100000101,
 *            a(9)=1100011011, a(10)=10100010101, a(11)=101101101011
 * Range:     u8 n <= 2, u16 n <= 4, u32 n <= 9, u64 n <= 19, u128 n <= 38,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>

static const seq_t init_vals[12] = {
    1ULL, 11ULL, 101ULL, 1011ULL, 10101ULL,
    101011ULL, 1000101ULL, 11011011ULL,
    100000101ULL, 1100011011ULL,
    10100010101ULL, 101101101011ULL
};

/* Allocate a cache large enough for the expected range of n (≈10000).  
   Adjust this size if larger n values are required. */
static seq_t cache[10001];
static idx_t max_computed = 0;
static int initialized = 0;

seq_t a(idx_t n) {
    /* Initialise cache with the first 12 terms once. */
    if (!initialized) {
        for (size_t i = 0; i < 12; ++i)
            cache[i] = init_vals[i];
        max_computed = 11;
        initialized = 1;
    }

    /* If the requested term is already computed, return it. */
    if (n <= max_computed)
        return cache[n];

    /* Extend the cache up to the required n. */
    for (idx_t i = max_computed + 1; i <= n; ++i) {
        /* a(i) = 100*a(i-2) + a(i-8) - 100*a(i-10)  (for i>11) */
        cache[i] = (seq_t)100 * cache[i - 2] + cache[i - 8] -
                  (seq_t)100 * cache[i - 10];
    }

    max_computed = n;
    return cache[n];
}
