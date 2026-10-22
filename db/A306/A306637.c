/*
 * A306637  a(n) = Fibonacci(n) * A128834(n).
 * https://oeis.org/A306637
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign, easy
 * Formula:   a(n) = Fibonacci(n) * A128834(n), where A128834(n) =
 *            {0,1,1,0,-1,-1} repeating with period 6
 * Range:     i8 n <= 12, i16 n <= 24, i32 n <= 43, i64 n <= 43, i128 n <= 43,
 *            big any n
 * Helpers:   fib
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/fib.h"

seq_t a(idx_t n)
{
    /* pattern for A128834: indexed by n mod 6 */
    static const int8_t pat[6] = {0, 1, 1, 0, -1, -1};
    idx_t m = n % 6;
    seq_t f = fib(n);               /* Fibonacci number F(n) */
    seq_t s = (seq_t)pat[m];        /* sign factor */
    return f * s;
}
