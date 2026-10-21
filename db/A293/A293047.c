/*
 * A293047  Number of odd permutations on {1,2,...,n} with exactly 2 weak
 *          excedances.
 * https://oeis.org/A293047
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, changed
 * Formula:   a(n) = 2^(n-1) - n (if n even), a(n) = 2^(n-1) - 1 (if n odd).
 *            a(0) = 0
 * Range:     u8 n <= 9, u16 n <= 17, u32 n <= 33, u64 n <= 34, u128 n <= 34,
 *            big any n
 * Helpers:   pow2
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/pow2.h"

seq_t a(idx_t n)
{
    if (n == 0)
        return 0;
    seq_t p = pow2(n - 1);                     /* compute 2^(n-1) */
    seq_t sub = (n & 1) ? (seq_t)1 : (seq_t)n; /* n if even, else 1   */
    return p - sub;
}
