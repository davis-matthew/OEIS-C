/*
 * A363879  Number of divisors of 7*n-6 of form 7*k+2.
 * https://oeis.org/A363879
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = number of divisors d of (7*n-6) such that d ≡ 2 (mod 7)
 * Range:     u8 n <= 100, u16 n <= 100, u32 n <= 100, u64 n <= 100,
 *            u128 n <= 100, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n)
{
    /* Guard for n==0 (undefined), return 0 */
    if (n == 0) return 0;

    /* Compute M = 7*n - 6 (fits in 64 bits for all practical n) */
    uint64_t M = (uint64_t)7 * (uint64_t)n - 6;

    seq_t count = 0;

    /* Enumerate divisors by scanning up to sqrt(M) */
    for (uint64_t d = 1; d * d <= M; ++d)
    {
        if (M % d == 0)
        {
            uint64_t q = M / d;
            if (d % 7 == 2) ++count;
            if (q != d && q % 7 == 2) ++count;
        }
    }

    return count;
}
