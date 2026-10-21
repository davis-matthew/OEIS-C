/*
 * A290756  Number of (non-null) connected induced subgraphs of the complete
 *          tripartite graph K_{n,n,n}.
 * https://oeis.org/A290756
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 8^n - 3*2^n + 3*n + 2
 * Range:     u8 n <= 2, u16 n <= 5, u32 n <= 10, u64 n <= 20, u128 n <= 20,
 *            big any n
 * Helpers:   mul_small, pow2
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/pow2.h"
#include "../utils/mul_small.h"

seq_t a(idx_t n)
{
    /* The sequence is defined for n >= 1. For n < 1 we return 0. */
    if (n < 1)
        return 0;

    /* Compute 8^n as 2^(3n) */
    idx_t three_n = n * 3;
    seq_t pow8 = pow2(three_n);

    /* Compute 2^n */
    seq_t pow2n = pow2(n);

    /* a(n) = 8^n - 3*2^n + 3*n + 2 */
    seq_t result = pow8;
    result -= mul_small(pow2n, 3);
    result += 3 * (seq_t)n;
    result += 2;

    return result;
}
