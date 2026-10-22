/*
 * A328920  let f(N) be the smallest index m such that from the m-th term on,
 *          the sequence {k^k mod N: k >= 0} enters into a cycle, then a(n) is
 *          the smallest number N such that f(N) = n, or 0 if no such n
 *          exists.
 * https://oeis.org/A328920
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(0)=1, a(1)=2. For n>1, let p be the smallest prime such that
 *            n-1 is divisible by p but not by p². Then a(n)=pⁿ; otherwise
 *            a(n)=0
 * Range:     u8 n <= 5, u16 n <= 7, u32 n <= 11, u64 n <= 17, u128 n <= 29,
 *            big any n
 * Helpers:   big_pow
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/big_pow.h"

seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)1;
    if (n == 1) return (seq_t)2;

    idx_t m = n - 1;
    idx_t tmp = m;
    idx_t p = 0;                 /* smallest prime with exponent 1 */

    /* factor 2 separately */
    if ((tmp & 1) == 0) {
        idx_t e = 0;
        while ((tmp & 1) == 0) {
            tmp >>= 1;
            ++e;
        }
        if (e == 1) p = 2;
    }

    /* factor odd numbers */
    for (idx_t f = 3; f * f <= tmp; f += 2) {
        if (tmp % f == 0) {
            idx_t e = 0;
            while (tmp % f == 0) {
                tmp /= f;
                ++e;
            }
            if (e == 1 && p == 0) p = f;
        }
    }

    /* leftover prime > sqrt(m) */
    if (tmp > 1 && p == 0) p = tmp;

    if (p == 0) return (seq_t)0;          /* no prime with exponent 1 */

    return big_pow((seq_t)p, (seq_t)n);   /* pⁿ */
}
