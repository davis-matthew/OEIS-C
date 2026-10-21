/*
 * A284245  Decimal representation of the x-axis, from the left edge to the
 *          origin, of the n-th stage of growth of the two-dimensional
 *          cellular automaton defined by "Rule 841", based on the 5-celled
 *          von Neumann neighborhood.
 * https://oeis.org/A284245
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = a(n-2) + 16*a(n-4) - 16*a(n-6) (for n > 16), with initial
 *            values a(0)=1, a(1)=0, a(2)=3, a(3)=7, a(4)=15, a(5)=29,
 *            a(6)=58, a(7)=125, a(8)=254, a(9)=471, a(10)=939, a(11)=2005,
 *            a(12)=4074, a(13)=7541, a(14)=15018, a(15)=32085, a(16)=65194
 * Range:     u8 n <= 8, u16 n <= 16, u32 n <= 32, u64 n <= 64, u128 n <= 126,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>      /* for size_t */

seq_t a(idx_t n)
{
    /* Static cache with capacity up to 10000 (the maximum n required). */
    static seq_t cache[10001];
    static int initialized = 0; /* 0 = not yet initialized, 1 = done */

    /* Initial 17 terms are stored once. */
    if (!initialized) {
        const seq_t init_vals[17] = {
            1ULL, 0ULL, 3ULL, 7ULL, 15ULL, 29ULL,
            58ULL, 125ULL, 254ULL, 471ULL, 939ULL,
            2005ULL, 4074ULL, 7541ULL, 15018ULL, 32085ULL, 65194ULL
        };
        for (size_t i = 0; i < 17; ++i)
            cache[i] = init_vals[i];

        /* All higher entries will be computed lazily. */
        initialized = 1;
    }

    /* Return immediately if n <= 16. */
    if (n <= 16)
        return cache[n];

    /* Compute terms up to n using the recurrence. */
    /* The loop starts at i = 17 because indices < 17 are already set. */
    for (size_t i = 17; i <= (size_t)n; ++i) {
        /* a(i) = a(i-2) + 16*a(i-4) - 16*a(i-6) */
        seq_t term = cache[i - 2];
        term += (seq_t)16 * cache[i - 4];
        term -= (seq_t)16 * cache[i - 6];
        cache[i] = term;
    }

    return cache[n];
}
