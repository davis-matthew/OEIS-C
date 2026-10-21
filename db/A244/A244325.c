/*
 * A244325  a(n) = floor(antisigma(n) / n), where antisigma(n) = A024816(n) =
 *          the sum of the non-divisors of n that are between 1 and n.
 * https://oeis.org/A244325
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = floor((n*(n+1)/2 - sigma(n)) / n)
 * Range:     u8 n <= 24, u16 n <= 697, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   sigma_of
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/sigma_of.h"

seq_t a(idx_t n)
{
    if (n == 0)
        return 0; /* undefined, but safe return */

    /* sum of 1..n as seq_t */
    seq_t sum = (seq_t)n * ((seq_t)n + 1) / 2;

    /* sigma(n) via provided utility */
    seq_t sig = sigma_of(n);

    /* antisigma = total sum minus divisors sum */
    seq_t antisigma = sum - sig;

    /* floor division by n */
    return antisigma / (seq_t)n;
}
