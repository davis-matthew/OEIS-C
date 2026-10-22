/*
 * A326003  G.f.: Sum_{n>=0} (n+1)*(n+2)/2 * x^n * (1 + x^n)^n.
 * https://oeis.org/A326003
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 11, u16 n <= 55, u32 n <= 263, u64 n <= 1223,
 *            u128 n <= 2050, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  OEIS A326003
    a(0) = 1
    for n > 0
        a(n) = Σ_{d | n, d>=1} C(d, n/d - 1) * (d + 1)(d + 2) / 2
    where C(d, k) is the binomial coefficient.
*/

#include <stdint.h>   /* for uint64_t, if needed */

/* Multiply-then-divide implementation of a binomial coefficient, safe for
   arbitrary-precision seq_t. */
static seq_t binomial(idx_t n, idx_t k)
{
    if (k > n) return 0;
    if (k > n - k) k = n - k;          /* take advantage of symmetry */
    seq_t res = 1;
    for (idx_t i = 1; i <= k; ++i) {
        res *= (seq_t)(n - k + i);
        res /= (seq_t)i;
    }
    return res;
}

seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)1;

    seq_t sum = 0;

    /* iterate over all divisors of n */
    for (idx_t i = 1; i <= n / i; ++i) {
        if (n % i != 0) continue;

        /* divisor d = i */
        {   idx_t d = i;
            idx_t k = n / d - 1;
            if (k <= d) {
                seq_t t = binomial(d, k);
                t *= ((seq_t)d + 1);
                t *= ((seq_t)d + 2);
                t /= 2;
                sum += t;
            }
        }

        /* divisor d = n / i, avoid double counting when i == n / i */
        idx_t d2 = n / i;
        if (d2 != i) {
            idx_t d = d2;
            idx_t k = n / d - 1;
            if (k <= d) {
                seq_t t = binomial(d, k);
                t *= ((seq_t)d + 1);
                t *= ((seq_t)d + 2);
                t /= 2;
                sum += t;
            }
        }
    }
    return sum;
}
