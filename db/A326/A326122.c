/*
 * A326122  a(n) = 10 * sigma(n).
 * https://oeis.org/A326122
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 10 * σ(n)
 * Range:     u8 n <= 11, u16 n <= 62, u32 n <= 62, u64 n <= 62, u128 n <= 62,
 *            big any n
 * Helpers:   sigma_of
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/sigma_of.h"

seq_t a(idx_t n)
{
    /* The OEIS offset is 1; for n = 0 return 0 safely. */
    if (n == 0)
        return (seq_t)0;
    return (seq_t)10 * sigma_of(n);
}
