/*
 * A367326  a(n) = Sum_{(n - k)|n} k^2.
 * https://oeis.org/A367326
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = n^2*A000005(n) - 2*n*A000203(n) + A001157(n)
 * Range:     u8 n <= 8, u16 n <= 48, u32 n <= 48, u64 n <= 48, u128 n <= 48,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 0) return 0;

    idx_t tmp = n;
    seq_t tau = 1;       /* number of divisors */
    seq_t sigma1 = 1;    /* sum of divisors */
    seq_t sigma2 = 1;    /* sum of squares of divisors */

    for (idx_t p = 2; p <= tmp / p; ++p) {
        if (tmp % p == 0) {
            idx_t exp = 0;
            while (tmp % p == 0) {
                tmp /= p;
                ++exp;
            }

            tau *= (seq_t)(exp + 1);

            /* contribution to sigma1: (p^(exp+1)-1)/(p-1) */
            seq_t num1 = 1;
            for (idx_t i = 0; i < exp + 1; ++i) num1 *= (seq_t)p;
            num1 -= 1;
            seq_t denom1 = (seq_t)(p - 1);
            sigma1 *= num1 / denom1;

            /* contribution to sigma2: (p^(2*(exp+1))-1)/(p^2-1) */
            seq_t num2 = 1;
            for (idx_t i = 0; i < 2 * (exp + 1); ++i) num2 *= (seq_t)p;
            num2 -= 1;
            seq_t denom2 = ((seq_t)p * (seq_t)p) - 1;
            sigma2 *= num2 / denom2;
        }
    }

    if (tmp > 1) {            /* remaining prime factor */
        idx_t p = tmp;
        tau *= 2;
        sigma1 *= ((seq_t)p + 1);
        sigma2 *= (seq_t)p * (seq_t)p + 1;
    }

    seq_t nn = (seq_t)n;
    seq_t result = nn * nn * tau - (seq_t)2 * nn * sigma1 + sigma2;
    return result;
}
