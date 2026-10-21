/*
 * A250605  Number of (n+1) X (2+1) 0..1 arrays with nondecreasing
 *          x(i,j)-x(i,j-1) in the i direction and nondecreasing
 *          x(i,j)-x(i-1,j) in the j direction.
 * https://oeis.org/A250605
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 36*2^(n-1) + n^2 - n - 10
 * Range:     u8 n <= 3, u16 n <= 11, u32 n <= 27, u64 n <= 59, u128 n <= 123,
 *            big any n
 * Helpers:   mul_small, pow2
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/pow2.h"
#include "../utils/mul_small.h"

seq_t a(idx_t n)
{
    /* Offset starts at 1; a(0) is undefined but we return 0 for safety. */
    if (n == 0)
        return 0;

    /* 36 * 2^(n-1) */
    seq_t term = pow2(n - 1);
    term = mul_small(term, 36);

    /* Add n^2, subtract n and 10 */
    seq_t ns = (seq_t)n;
    term += ns * ns;  /* n^2 */
    term -= ns;       /* - n */
    term -= 10;       /* -10 */

    return term;
}
