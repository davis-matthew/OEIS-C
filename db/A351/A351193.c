/*
 * A351193  Sum of the 5th powers of primes dividing n.
 * https://oeis.org/A351193
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Sum_{p|n, p prime} p^5
 * Range:     u8 n <= 4, u16 n <= 10, u32 n <= 88, u64 n <= 7150,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n)
{
    if (n <= 1)
        return 0;

    idx_t m = n;
    seq_t sum = 0;

    for (idx_t p = 2; p * p <= m; ++p) {
        if (m % p == 0) {
            seq_t p5 = (seq_t)p;
            p5 *= (seq_t)p;  /* p^2 */
            p5 *= (seq_t)p;  /* p^3 */
            p5 *= (seq_t)p;  /* p^4 */
            p5 *= (seq_t)p;  /* p^5 */
            sum += p5;
            while (m % p == 0)
                m /= p;
        }
    }

    if (m > 1) {                 /* remaining prime factor */
        seq_t p5 = (seq_t)m;
        p5 *= (seq_t)m;
        p5 *= (seq_t)m;
        p5 *= (seq_t)m;
        p5 *= (seq_t)m;
        sum += p5;
    }

    return sum;
}
