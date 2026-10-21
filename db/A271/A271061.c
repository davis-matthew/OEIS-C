/*
 * A271061  Number of active (ON, black) cells at stage 2^n-1 of the
 *          two-dimensional cellular automaton defined by "Rule 261", based on
 *          the 5-celled von Neumann neighborhood.
 * https://oeis.org/A271061
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, more
 * Formula:   a(n) = 1, n = 0; for n > 0 a(n) = 4*4^n - 4*2^n
 * Range:     u8 n <= 3, u16 n <= 7, u32 n <= 15, u64 n <= 15, u128 n <= 15,
 *            big any n
 * Helpers:   pow2
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Uses: pow2 helper to compute 2^n and basic arithmetic. */
#include "../utils/pow2.h"

seq_t a(idx_t n)
{
    if (n == 0)
        return (seq_t)1;

    seq_t two_pow_n = pow2(n);          /* 2^n  */
    seq_t two_pow_2n = two_pow_n * two_pow_n; /* 2^(2n) = (2^n)^2 */

    /* a(n) = 4*(2^(2n) - 2^n) */
    seq_t result = 4 * (two_pow_2n - two_pow_n);
    return result;
}
