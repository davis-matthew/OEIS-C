/*
 * A386811  a(n) = Sum_{k=0..n} binomial(4*n+1,k).
 * https://oeis.org/A386811
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Sum_{k=0..n} binomial(4*n+1,k)
 * Range:     u8 n <= 2, u16 n <= 5, u32 n <= 10, u64 n <= 20, u128 n <= 40,
 *            big any n
 * Helpers:   gcd
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/gcd.h"

seq_t a(idx_t n)
{
    seq_t sum = 0;
    idx_t m = 4 * n + 1;          /* upper limit of the binomial coefficient */

    seq_t c = 1;                 /* C(m,0) */

    for (idx_t k = 0; k <= n; ++k) {
        sum += c;                 /* add current binomial coefficient */

        if (k == n)              /* last term reached; stop */
            break;

        /* next binomial C(m,k+1) = C(m,k) * (m-k) / (k+1)
           Reduce the fraction to avoid overflow when possible. */
        seq_t num = (seq_t)(m - k);
        seq_t den = (seq_t)(k + 1);

        /* Simplify c/den */
        seq_t g1 = gcd(c, den);
        c   = c / g1;
        den = den / g1;

        /* Simplify num/den */
        seq_t g2 = gcd(num, den);
        num = num / g2;
        den = den / g2;           /* den should now be 1 */

        c = c * num;              /* final multiplication */
    }

    return sum;
}
