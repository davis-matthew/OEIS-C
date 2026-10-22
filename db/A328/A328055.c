/*
 * A328055  Expansion of e.g.f. -log(1 - x / (1 - x)^2).
 * https://oeis.org/A328055
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(0) = 0; for n > 0, a(n) = (n-1)! * (Lucas(2*n) - 2)
 * Range:     u8 n <= 3, u16 n <= 6, u32 n <= 9, u64 n <= 16, u128 n <= 19,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*          Lucas(m) = 2*Fib(m+1) - Fib(m)   */
#include <stddef.h>

static void fib_pair(idx_t n, seq_t *f, seq_t *f1)
/* computes F(n)=Fib(n) and F(n+1) = Fib(n+1) */
{
    if (n == 0) {
        *f  = 0;
        *f1 = 1;
        return;
    }

    seq_t a, b;
    fib_pair(n >> 1, &a, &b);

    seq_t two_b = b * (seq_t)2;
    seq_t c = a * (two_b - a);
    seq_t d = a * a + b * b;

    if ((n & 1) == 0) {
        *f  = c;
        *f1 = d;
    } else {
        *f  = d;
        *f1 = c + d;
    }
}

seq_t a(idx_t n)
{
    if (n == 0)
        return 0;

    /* factorial part: (n-1)! */
    seq_t fact = 1;
    for (idx_t i = 2; i <= n - 1; ++i)
        fact = fact * (seq_t)i;

    /* Lucas(2*n)  = 2*Fib(2*n+1) - Fib(2*n)  */
    idx_t m = 2 * n;
    seq_t fm, fm1;
    fib_pair(m, &fm, &fm1);          /* fm = Fib(2*n), fm1 = Fib(2*n+1) */
    seq_t luc = fm1 * (seq_t)2 - fm;

    return fact * (luc - (seq_t)2);
}
