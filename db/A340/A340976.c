/*
 * A340976  a(n) = Sum_{1 < k < n} sigma(n) mod k, where sigma = A000203.
 * https://oeis.org/A340976
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Sum_{1 < k < n} sigma(n) mod k, where sigma = sum of
 *            divisors of n (OEIS A000203)
 * Range:     u8 n <= 16, u16 n <= 63, u32 n <= 63, u64 n <= 63, u128 n <= 63,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Compute sigma(n) by prime factorisation and then sum remainders. */

#include <stdint.h>

static seq_t _sigma_of(idx_t n)
{
    seq_t sigma = 1;
    idx_t tmp = n;
    idx_t d = 2;

    /* factorise tmp */
    while (d <= tmp / d) {
        if (tmp % d == 0) {
            /* find exponent e of prime d */
            idx_t e = 0;
            while (tmp % d == 0) {
                tmp /= d;
                ++e;
            }

            /* compute d^(e+1) */
            seq_t d_pow = 1;
            for (idx_t i = 0; i < e + 1; ++i)
                d_pow *= (seq_t)d;

            /* sum of powers: (d^(e+1)-1)/(d-1) */
            seq_t sum_d = (d_pow - 1) / ((seq_t)d - 1);

            sigma *= sum_d;
        }
        ++d;
    }

    /* remaining prime factor, if any */
    if (tmp > 1) {
        seq_t d_pow = tmp * tmp;                 /* tmp^(1+1) */
        seq_t sum_d = (d_pow - 1) / ((seq_t)tmp - 1);
        sigma *= sum_d;
    }

    return sigma;
}

seq_t a(idx_t n)
{
    if (n <= 2)
        return 0;    /* a(1)=0, a(2)=0 */

    seq_t sigma_n = _sigma_of(n);
    seq_t sum = 0;

    for (idx_t k = 2; k < n; ++k)
        sum += sigma_n % (seq_t)k;

    return sum;
}
