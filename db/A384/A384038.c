/*
 * A384038  Number of 2n X 2n matrices M over GF(2) such that the column space
 *          of M is equal to the null space of M.
 * https://oeis.org/A384038
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 2^{n(n-1)/2} * Π_{j=n+1}^{2n} (2^j - 1)
 * Range:     u8 n <= 2, u16 n <= 2, u32 n <= 4, u64 n <= 5, u128 n <= 8,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>

seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)1;

    seq_t result = (seq_t)1;
    /* Compute Π_{j=n+1}^{2n} (2^j - 1) */
    for (idx_t j = n + 1; j <= n + n; ++j)
    {
        seq_t term = ((seq_t)1 << j) - (seq_t)1;
        result *= term;
    }

    /* Multiply by 2^{n(n-1)/2} */
    idx_t exp = n * (n - 1) / 2;
    seq_t pow2 = ((seq_t)1 << exp);
    result *= pow2;

    return result;
}
