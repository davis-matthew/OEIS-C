/*
 * A310532  Coordination sequence Gal.6.333.1 where Gal.u.t.v denotes the
 *          coordination sequence for a vertex of type v in tiling number t in
 *          the Galebach list of u-uniform tilings.
 * https://oeis.org/A310532
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 2*a(n-1) - 2*a(n-2) + 2*a(n-3) - 2*a(n-4) + 2*a(n-5) - *
 *            2*a(n-6) + 2*a(n-7) - 2*a(n-8) + 2*a(n-9) - a(n-10) for n > 10 *
 *            Initial values: a(0)=1, a(1)=4, a(2)=10, a(3)=16, a(4)=22, *
 *            a(5)=28, a(6)=34, a(7)=40, a(8)=46, a(9)=52, a(10)=56 * This
 *            function works for any unsigned seq_t type and...
 * Range:     u8 n <= 45, u16 n <= 49, u32 n <= 49, u64 n <= 49, u128 n <= 49,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>   /* for size_t */

seq_t a(idx_t n)
{
    /* Initial values up to a(10). */
    static const seq_t base[11] = {
        1ULL, 4ULL, 10ULL, 16ULL, 22ULL,
        28ULL, 34ULL, 40ULL, 46ULL, 52ULL, 56ULL
    };

    if (n <= 10)
        return base[(size_t)n];

    /* Sliding window of the last 11 terms. */
    seq_t prev[11];
    for (size_t i = 0; i < 11; ++i)
        prev[i] = base[i];

    for (idx_t i = 11; i <= n; ++i)
    {
        /* Compute the next term from the recurrence. */
        seq_t val =  2 * prev[10] - 2 * prev[9]
                   + 2 * prev[8] - 2 * prev[7]
                   + 2 * prev[6] - 2 * prev[5]
                   + 2 * prev[4] - 2 * prev[3]
                   + 2 * prev[2] - prev[1];

        /* Shift the window left by one position. */
        for (size_t k = 0; k < 10; ++k)
            prev[k] = prev[k + 1];
        prev[10] = val;
    }

    return prev[10];
}
