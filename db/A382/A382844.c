/*
 * A382844  Area of the unique primitive Pythagorean triple (a,b,c) such that
 *          (a-b+c)/2 = A000045(n) and its long leg and hypotenuse are
 *          consecutive natural numbers.
 * https://oeis.org/A382844
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = Fibonacci(n)*(Fibonacci(n)-1)*(2*Fibonacci(n)-1)
 * Range:     u8 n <= 5, u16 n <= 8, u32 n <= 16, u64 n <= 31, u128 n <= 62,
 *            big any n
 * Helpers:   fib, seq_mul
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>
#include "../utils/fib.h"
#include "../utils/seq_mul.h"

seq_t a(idx_t n)
{
    seq_t f = fib(n);           /* Fibonacci(n) */
    if (f == 0) return 0;       /* handles n = 0,1,2 */

    seq_t f_minus1 = f - 1;                     /* Fibonacci(n)-1 */
    seq_t two_f = seq_mul(f, (seq_t)2);          /* 2*Fibonacci(n) */
    seq_t two_f_minus1 = two_f - 1;              /* 2*Fibonacci(n)-1 */

    seq_t prod = seq_mul(f, f_minus1);          /* f * (f-1) */
    prod = seq_mul(prod, two_f_minus1);         /* * (2f-1) */

    return prod;
}
