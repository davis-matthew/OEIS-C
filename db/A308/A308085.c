/*
 * A308085  a(n) is the least positive number k such that n*(n-1)/2 +
 *          k*(k-1)/2 is a square.
 * https://oeis.org/A308085
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, look
 * Formula:   a(n) = least positive k such that n*(n-1)/2 + k*(k-1)/2 is a
 *            perfect square. For n > 1, a(n) <= n-1 (see OEIS note)
 * Range:     u8 n <= 9, u16 n <= 82, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   floor_sqrt
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/floor_sqrt.h"

seq_t a(idx_t n)
{
    /* Special case: the first term of the sequence */
    if (n == 1) return 1;

    /* Triangular number of n: T(n-1) = n*(n-1)/2 */
    const seq_t tri_n = (seq_t)n * ((seq_t)n - 1) / 2;

    /* Upper bound for k: a(n) <= n-1 for n > 1 */
    const idx_t limit = n - 1;

    for (idx_t k = 1; k <= limit; ++k)
    {
        const seq_t tri_k = (seq_t)k * ((seq_t)k - 1) / 2;
        const seq_t sum   = tri_n + tri_k;

        const seq_t s = floor_sqrt(sum);
        if (s * s == sum)   /* perfect square found */
            return (seq_t)k;
    }

    /* In theory this point is never reached because a(n) <= n-1 */
    return (seq_t)limit;
}
