/*
 * A337194  a(n) = 1 + A000265(sigma(n)), where A000265 gives the odd part.
 * https://oeis.org/A337194
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 1 + A000265(σ(n)) where σ(n) = sum of divisors of n and
 *            A000265(m) is the odd part of m (divide by all factors of 2)
 * Range:     u8 n <= 127, u16 n <= 9869, u32 n <= 9869, u64 n <= 9869,
 *            u128 n <= 9869, big any n
 * Helpers:   sigma_of
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/sigma_of.h"

seq_t a(idx_t n)
{
    /* Compute σ(n). */
    seq_t s = sigma_of(n);

    /* Strip all powers of 2: obtain odd_part(s). */
    seq_t odd = s;
    while ((odd % 2) == 0)
        odd /= 2;

    /* a(n) = 1 + odd_part(s). */
    return odd + 1;
}
