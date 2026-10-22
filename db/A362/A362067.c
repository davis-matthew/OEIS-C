/*
 * A362067  Sum of successive Fibonacci numbers F(n) : a(n) = Sum_{k = 0..n}
 *          F(n+k).
 * https://oeis.org/A362067
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = F(2n+2) - F(n+1) (F(0)=0, F(1)=1)
 * Range:     u8 n <= 5, u16 n <= 11, u32 n <= 22, u64 n <= 45, u128 n <= 92,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>

static seq_t fib_uint(idx_t m)
{
    /* Return the m-th Fibonacci number (F(0)=0, F(1)=1). */
    if (m == 0) return (seq_t)0;
    if (m == 1) return (seq_t)1;
    seq_t a = (seq_t)0;
    seq_t b = (seq_t)1;
    /* Use idx_t for the loop counter so that all arithmetic on a,b is done
       in seq_t, preventing any overflow that might arise from casting m to
       seq_t before the loop starts. */
    for (idx_t i = 2; i <= m; ++i) {
        seq_t c = a + b;
        a = b;
        b = c;
    }
    return b;
}

seq_t a(idx_t n)
{
    seq_t fn1 = fib_uint(n + 1);          /* F(n+1) */
    seq_t f2n2 = fib_uint(n + n + 2);     /* F(2n+2) */
    return f2n2 - fn1;
}
