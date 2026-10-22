/*
 * A340728  a(n) is the number of divisors d of n such that n/d - d is prime.
 * https://oeis.org/A340728
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = ∑_{d|n} c(n/d - d) where c(m) = 1 if m is prime, 0
 *            otherwise
 * Range:     u8 n <= 63, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   floor_sqrt, is_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/is_prime.h"
#include "../utils/floor_sqrt.h"

seq_t a(idx_t n)
{
    /* For n = 1 the result is 0, this also handles n = 0 uniformly. */
    if (n <= 1) return 0;

    /* Work in uint64_t since n is an idx_t (unsigned 64‑bit). */
    uint64_t nn = (uint64_t)n;

    /* Compute ⌊√n⌋ using the provided utility. */
    uint64_t sqrt_n = (uint64_t)floor_sqrt((seq_t)nn);

    seq_t count = 0;

    /* Enumerate all divisors d ≤ √n.  For each such d we have the
       complementary divisor q = n/d, and the difference q - d is
       always non‑negative.  If this difference is a prime > 1,
       increment the counter. */
    for (uint64_t d = 1; d <= sqrt_n; ++d) {
        if (nn % d == 0) {
            uint64_t q    = nn / d;
            uint64_t diff = q - d;
            if (diff > 1 && is_prime((idx_t)diff))
                count++;
        }
    }

    return count;
}
