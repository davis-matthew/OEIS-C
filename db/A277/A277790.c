/*
 * A277790  Numerator of sum of reciprocals of proper divisors of n.
 * https://oeis.org/A277790
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, frac
 * Formula:   a(n) = numerator((sigma(n)-1)/n) (sum of reciprocals of * proper
 *            divisors of n, numerator after reducing the fraction)
 * Range:     u8 n <= 119, u16 n <= 9728, u32 n <= 9728, u64 n <= 9728,
 *            u128 n <= 9728, big any n
 * Helpers:   gcd
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/gcd.h"

seq_t a(idx_t n)
{
    if (n == 1)          /* 1 has no proper divisors, a(1)=0 */
        return 0;

    /* Compute the sum of all divisors σ(n)  */
    seq_t sigma = 1;     /* start with divisor 1          */
    idx_t  i;

    for (i = 2; i * i <= n; ++i) {
        if (n % i == 0) {
            seq_t d1 = (seq_t)i;
            seq_t d2 = (seq_t)(n / i);
            if (d1 == d2)          /* i is a square root of n   */
                sigma += d1;
            else
                sigma += d1 + d2;   /* add both divisors         */
        }
    }
    sigma += (seq_t)n;   /* add n itself                 */

    /* numerator of (σ(n)-1)/n, reduced   */
    seq_t num = sigma - 1;
    seq_t g   = gcd(num, (seq_t)n);
    return num / g;
}
