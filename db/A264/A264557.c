/*
 * A264557  Number of n X 1 arrays of permutations of 0..n*1-1 with rows
 *          nondecreasing modulo 2 and columns nondecreasing modulo 3.
 * https://oeis.org/A264557
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = ceil(n/3)! * ceil((n-1)/3)! * ceil((n-2)/3)!
 * Range:     u8 n <= 9, u16 n <= 12, u32 n <= 19, u64 n <= 29, u128 n <= 46,
 *            big any n
 * Helpers:   ceil_div3, factorial
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/factorial.h"
#include "../utils/ceil_div3.h"

/* External function required by the build system. */
seq_t a(idx_t n)
{
    /* Convert to signed for possible subtraction underflow. */
    int64_t k1 = (int64_t)n;
    int64_t k2 = k1 - 1;
    int64_t k3 = k1 - 2;

    /* Ceiling divisions. */
    idx_t c1 = ceil_div3(k1);
    idx_t c2 = ceil_div3(k2);
    idx_t c3 = ceil_div3(k3);

    /* Compute the product of factorials. */
    seq_t res = factorial(c1);
    res *= factorial(c2);
    res *= factorial(c3);

    return res;
}
