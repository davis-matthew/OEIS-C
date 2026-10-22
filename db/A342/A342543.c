/*
 * A342543  a(n) = Sum_{k=1..n} phi(gcd(k, n))^gcd(k, n).
 * https://oeis.org/A342543
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Sum_{k=1..n} phi(gcd(k, n))^gcd(k, n)
 * Range:     u8 n <= 4, u16 n <= 6, u32 n <= 10, u64 n <= 16, u128 n <= 28,
 *            big any n
 * Helpers:   big_pow, phi
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*         = Sum_{d|n} phi(n/d) * phi(d)^d  (OEIS notation) */

#include "../utils/phi.h"
#include "../utils/big_pow.h"

/* Computes the n-th term of OEIS sequence A342543. */
seq_t a(idx_t n)
{
    seq_t sum = 0;
    for (idx_t i = 1; i * i <= n; ++i)
    {
        if (n % i == 0)
        {
            idx_t d1 = i;
            idx_t d2 = n / i;

            seq_t phi_d1 = phi(d1);
            seq_t phi_d2 = phi(d2);

            seq_t pow_d1 = big_pow(phi_d1, (seq_t)d1);
            sum += phi_d2 * pow_d1;

            if (d1 != d2)
            {
                seq_t pow_d2 = big_pow(phi_d2, (seq_t)d2);
                sum += phi_d1 * pow_d2;
            }
        }
    }
    return sum;
}
