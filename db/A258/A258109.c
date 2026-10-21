/*
 * A258109  Number of balanced parenthesis expressions of length 2n and depth
 *          3.
 * https://oeis.org/A258109
 *
 * Offset:    3    (a(3) is the first term)
 * Keywords:  nonn, walk, easy
 * Formula:   a(n) = fib(2*n-1) - 2^(n-1) (valid for n >= 3)
 * Range:     u8 n <= 7, u16 n <= 12, u32 n <= 24, u64 n <= 47, u128 n <= 93,
 *            big any n
 * Helpers:   fib, pow2
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/fib.h"
#include "../utils/pow2.h"

seq_t a(idx_t n)
{
    if (n < 3) /* offset not used; return 0 or undefined */
        return 0;
    /* use 2*n - 1 to avoid overflow on very large n */
    idx_t fib_index = (idx_t)(2 * n - 1);
    seq_t f = fib(fib_index);
    seq_t p = pow2((idx_t)(n - 1));
    return f - p;
}
