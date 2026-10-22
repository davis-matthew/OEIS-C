/*
 * A374455  Expansion of o.g.f. 1/(1 - 5*x - 10*x^2 - 10*x^3 - 5*x^4 - x^5).
 * https://oeis.org/A374455
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 5*a(n-1) + 10*a(n-2) + 10*a(n-3) + 5*a(n-4) + a(n-5),
 *            initial values: a(0)=1, a(1)=5, a(2)=35, a(3)=235, a(4)=1580
 * Range:     u8 n <= 3, u16 n <= 5, u32 n <= 11, u64 n <= 22, u128 n <= 22,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>

static inline seq_t mul_const(seq_t x, unsigned int m)
{
    seq_t res = 0;
    while (m) {
        if (m & 1)
            res += x;
        x += x;
        m >>= 1;
    }
    return res;
}

seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)1;
    if (n == 1) return (seq_t)5;
    if (n == 2) return (seq_t)35;
    if (n == 3) return (seq_t)235;
    if (n == 4) return (seq_t)1580;

    seq_t a0 = (seq_t)1,  a1 = (seq_t)5,  a2 = (seq_t)35,
          a3 = (seq_t)235, a4 = (seq_t)1580;

    for (idx_t i = 5; i <= n; ++i) {
        seq_t a5 = mul_const(a4, 5);
        a5 += mul_const(a3, 10);
        a5 += mul_const(a2, 10);
        a5 += mul_const(a1, 5);
        a5 += a0;

        a0 = a1;
        a1 = a2;
        a2 = a3;
        a3 = a4;
        a4 = a5;
    }
    return a4;
}
