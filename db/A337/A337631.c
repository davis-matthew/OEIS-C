/*
 * A337631  a(n) is the sum of the squares of diameters of all nonempty
 *          subsets of {1,2,...,n}.
 * https://oeis.org/A337631
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 2^n*(n^2 - 6*n + 13) - 3*n - 13
 * Range:     u8 n <= 5, u16 n <= 10, u32 n <= 23, u64 n <= 52, u128 n <= 114,
 *            big any n
 * Helpers:   pow2
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*        a(1) = 0, a(2) = 1, a(3) = 10, ... (see OEIS A337631) */
#include "../utils/pow2.h"

seq_t a(idx_t n)
{
    /* n == 0 is outside the defined offset; return 0. */
    if (n == 0) return 0;

    seq_t N = (seq_t)n;           /* cast index to seq_t            */
    seq_t pow = pow2(n);          /* 2^n                            */
    seq_t expr = N * N - (seq_t)6 * N + (seq_t)13; /* n^2 - 6n + 13 */
    seq_t result = pow * expr - (seq_t)3 * N - (seq_t)13;
    return result;
}
