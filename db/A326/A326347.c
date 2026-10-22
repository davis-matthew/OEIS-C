/*
 * A326347  Number of unordered pairs of 4-colorings of an n-cycle that differ
 *          in the coloring of exactly one vertex.
 * https://oeis.org/A326347
 *
 * Offset:    3    (a(3) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 6*n*(3^(n-2) + (-1)^n)
 * Range:     u8 n <= 4, u16 n <= 8, u32 n <= 17, u64 n <= 37, u128 n <= 77,
 *            big any n
 * Helpers:   pow3
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/pow3.h"

seq_t a(idx_t n)
{
    /* The sequence is defined only for n ≥ 3. */
    if (n < 3)
        return 0;

    /* Compute 3^(n-2) using the helper pow3, which returns a seq_t. */
    seq_t pow3v = pow3(n - 2);

    /* Add or subtract 1 depending on the parity of n. */
    seq_t sum;
    if ((n & 1) == 0)          /* n even */
        sum = pow3v + 1;
    else                       /* n odd */
        sum = pow3v - 1;        /* pow3v ≥ 3 for n ≥ 3, so subtraction is safe */

    /* Multiply by 6*n. */
    seq_t result = (seq_t)n * 6;
    result *= sum;

    return result;
}
