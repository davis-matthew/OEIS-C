/*
 * A271358  a(n) = k*Fibonacci(2*n+1) + (k+1)*Fibonacci(2*n), where k=4.
 * https://oeis.org/A271358
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 3*a(n-1) - a(n-2); a(0) = 4, a(1) = 13
 * Range:     u8 n <= 4, u16 n <= 9, u32 n <= 21, u64 n <= 44, u128 n <= 90,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>   /* for bool if needed */

seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)4;
    if (n == 1) return (seq_t)13;

    seq_t a0 = (seq_t)4;
    seq_t a1 = (seq_t)13;
    seq_t ai = a1;

    for (idx_t i = 2; i <= n; ++i) {
        ai = (seq_t)3 * a1 - a0;
        a0 = a1;
        a1 = ai;
    }
    return ai;
}
