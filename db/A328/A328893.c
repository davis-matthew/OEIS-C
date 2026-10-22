/*
 * A328893  Partial sums of A095112: sum of the Dirichlet convolution of the
 *          characteristic function of the prime powers (A069513) with the
 *          positive integers (A000027) from 1 to n.
 * https://oeis.org/A328893
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Sum_{p^k <= n} floor(n / p^k) * (floor(n / p^k) + 1) / 2
 * Range:     u8 n <= 29, u16 n <= 57, u32 n <= 57, u64 n <= 57, u128 n <= 57,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* where the sum runs over all prime powers p^k <= n. */

#include <stdbool.h>

seq_t a(idx_t n)
{
    seq_t sum = (seq_t)0;
    for (idx_t p = 2; p <= n; ++p) {
        /* test primality of p by trial division */
        bool isprime = true;
        for (idx_t d = 2; d * d <= p; ++d) {
            if (p % d == 0) {
                isprime = false;
                break;
            }
        }
        if (!isprime) continue;

        idx_t power = p;
        while (power <= n) {
            idx_t t = n / power;              /* floor(n / p^k) */
            seq_t tt = (seq_t)t;
            sum += tt * (tt + (seq_t)1) / (seq_t)2;  /* t*(t+1)/2 */

            /* avoid overflow when multiplying power by p */
            if (power > n / p) break;
            power *= p;
        }
    }
    return sum;
}
