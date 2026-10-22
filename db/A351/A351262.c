/*
 * A351262  a(n) = n^10 * Sum_{p|n, p prime} 1/p^10.
 * https://oeis.org/A351262
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = ∑_{p|n} (n/p)^10 (sum over distinct prime divisors p)
 * Range:     u8 n <= 3, u16 n <= 7, u32 n <= 19, u64 n <= 169,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

static inline seq_t pow10_seq(seq_t base)
{
    seq_t r = (seq_t)1;
    for (idx_t i = 0; i < 10; ++i)
        r *= base;
    return r;
}

seq_t a(idx_t n)
{
    if (n <= 1)
        return (seq_t)0;          /* a(1)=0, a(0) not used */

    /* keep the original value for qp = n/p */
    const idx_t orig = n;
    seq_t sum = (seq_t)0;

    /* trial division to find distinct prime factors */
    idx_t m = n;
    for (idx_t p = 2; p * p <= m; ++p)
    {
        if (m % p == 0)
        {
            /* p is a prime divisor; compute (n/p)^10 */
            seq_t qp = (seq_t)(orig / p);
            sum += pow10_seq(qp);

            /* remove all powers of p */
            while (m % p == 0)
                m /= p;
        }
    }
    if (m > 1)          /* remaining prime factor larger than sqrt(orig) */
    {
        seq_t qp = (seq_t)(orig / m);
        sum += pow10_seq(qp);
    }

    return sum;
}
