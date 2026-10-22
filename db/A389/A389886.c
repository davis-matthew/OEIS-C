/*
 * A389886  Number of proper 3-partitions of the vertices of the fan graph on
 *          n vertices such that every triangle uses exactly 2 colors.
 * https://oeis.org/A389886
 *
 * Offset:    3    (a(3) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 3*2^(n-3) - F_(n+1)
 * Range:     u8 n <= 9, u16 n <= 17, u32 n <= 33, u64 n <= 65, u128 n <= 129,
 *            big any n
 * Helpers:   fib, pow2
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>
#include "../utils/fib.h"
#include "../utils/pow2.h"

seq_t a(idx_t n)
{
    /* For n < 3 the sequence is not defined (offset 3).  Return 0. */
    if (n < 3)
        return 0;

    /* 2^(n-3) as a seq_t value */
    seq_t pow_val = pow2(n - 3);

    /* 3*2^(n-3) */
    seq_t term = 3 * pow_val;

    /* Fibonacci number F_(n+1) */
    seq_t fib_val = fib(n + 1);

    /* a(n) = 3*2^(n-3) - F_(n+1) */
    return term - fib_val;
}
