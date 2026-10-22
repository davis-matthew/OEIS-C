/*
 * A382308  Sum of the legs of the unique primitive Pythagorean triple whose
 *          inradius is A000045(n) and such that its long leg and its
 *          hypotenuse are consecutive natural numbers.
 * https://oeis.org/A382308
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 2*(Fibonacci(n))^2 + 4*Fibonacci(n) + 1
 * Range:     u8 n <= 6, u16 n <= 12, u32 n <= 23, u64 n <= 47, u128 n <= 93,
 *            big any n
 * Helpers:   _fib_fast
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/_fib_fast.h"

seq_t a(idx_t n)
{
    seq_t f = _fib_fast(n);              /* Fibonacci(n) */
    seq_t result = 2 * f * f + 4 * f + 1;
    return result;
}
