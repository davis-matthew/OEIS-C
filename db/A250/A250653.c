/*
 * A250653  Number of (n+1)X(5+1) 0..1 arrays with nondecreasing
 *          x(i,j)-x(i,j-1) in the i direction and nondecreasing
 *          min(x(i,j),x(i-1,j)) in the j direction.
 * https://oeis.org/A250653
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 27*2^n - 5 (offset 1)
 * Range:     u8 n <= 3, u16 n <= 11, u32 n <= 27, u64 n <= 59, u128 n <= 123,
 *            big any n
 * Helpers:   pow2
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/pow2.h"

seq_t a(idx_t n)
{
    /* For n < 1 the sequence is undefined; return 0. */
    if (n < 1)
        return (seq_t)0;

    seq_t v = pow2(n);          /* v = 2^n */
    v = v * (seq_t)27;          /* v = 27*2^n */
    v = v - (seq_t)5;           /* v = 27*2^n - 5 */
    return v;
}
