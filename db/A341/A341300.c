/*
 * A341300  a(n) = n! + (n-1)! + (n-2)! + (n-3)! + n - 3.
 * https://oeis.org/A341300
 *
 * Offset:    3    (a(3) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = (n-3)!*(n^2*(n-2)+1) + n - 3 (OEIS A341300)
 * Range:     u8 n <= 5, u16 n <= 8, u32 n <= 12, u64 n <= 20, u128 n <= 34,
 *            big any n
 * Helpers:   factorial
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/factorial.h"

seq_t a(idx_t n)
{
    if (n < 3) return 0;          /* defined only for n ≥ 3 */
    idx_t m = n - 3;

    /* Compute (n-3)! using the supplied factorial routine */
    seq_t fact = factorial(m);

    /* Compute the multiplier: n^2*(n-2)+1  using seq_t arithmetic */
    seq_t tmp = (seq_t)n * (seq_t)n * (seq_t)(n - 2) + (seq_t)1;

    /* (n-3)! * (n^2*(n-2)+1) */
    seq_t result = fact * tmp;

    /* + n - 3 */
    result += (seq_t)n;
    result -= (seq_t)3;

    return result;
}
