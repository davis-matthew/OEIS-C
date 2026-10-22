/*
 * A382845  Sum of the legs of the unique primitive Pythagorean triple (a,b,c)
 *          such that (a-b+c)/2 = A000045(n) and its long leg and hypotenuse
 *          are consecutive natural numbers.
 * https://oeis.org/A382845
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign, easy
 * Formula:   a(n) = 2*Fibonacci(n)^2 - 1
 * Range:     i8 n <= 6, i16 n <= 11, i32 n <= 23, i64 n <= 46, i128 n <= 92,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* Compute Fibonacci(n) using an iterative loop.
       f0  holds Fib(k), f1  holds Fib(k+1).  After n iterations,
       f0 becomes Fib(n). */
    seq_t f0 = 0;   /* Fib(0)   */
    seq_t f1 = 1;   /* Fib(1)   */

    for (idx_t i = 0; i < n; ++i)
    {
        seq_t next = f0 + f1;
        f0 = f1;
        f1 = next;
    }

    /* f0 now equals Fibonacci(n).  Compute 2*f0^2 - 1. */
    seq_t result = f0 * f0;
    result = result * 2;
    result = result - 1;
    return result;
}
