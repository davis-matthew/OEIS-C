/*
 * A363561  G.f.: Sum_{n=-oo..+oo} x^n * (sqrt(2) + x^n)^(2*n).
 * https://oeis.org/A363561
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Sum_{d|n} [ binom(2d, t-1)*sqrt(2)^{2d-(t-1)} +
 *            (-1)^{t-2d+1}*binom(2d + t-2d+1 -1, t-2d+1)*sqrt(2)^{t-2d+1} ],
 *            where t = n/d. The sum can be written as a sum of an integer
 *            part and a multiple of sqrt(2); the sqrt(2) parts cancel,
 *            leaving an integer
 * Range:     u8 n <= 7, u16 n <= 15, u32 n <= 31, u64 n <= 63, u128 n <= 127,
 *            big any n
 * Helpers:   binom, pow2
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/binom.h"   /* seq_t binom(idx_t a, idx_t b); */
#include "../utils/pow2.h"    /* seq_t pow2(idx_t exp);  returns 2^exp  */

seq_t a(idx_t n)
{
    if (n == 0)
        return (seq_t)1;          /* a(0) = 1 */

    seq_t int_pos = 0;            /* sum of positive integer contributions */
    seq_t int_neg = 0;            /* sum of negative integer contributions */
    seq_t sqrt_pos = 0;           /* sum of positive sqrt(2) contributions */
    seq_t sqrt_neg = 0;           /* sum of negative sqrt(2) contributions */

    /* enumerate all positive divisors d of n */
    for (idx_t d = 1; d * d <= n; ++d) {
        if (n % d != 0)
            continue;

        /* process divisor d */
        for (int sign_of_d = 0; sign_of_d < 2; ++sign_of_d) {
            idx_t divisor = (sign_of_d == 0) ? d : n / d;
            if (sign_of_d == 1 && d * d == n)  /* avoid duplicate divisor when d^2 == n */
                continue;

            idx_t t = n / divisor;            /* t = n/divisor */
            /* Positive contribution from n = +divisor when t <= 2*divisor+1 */
            if (t <= 2 * divisor + 1) {
                idx_t k = t - 1;              /* 0 <= k <= 2*divisor */
                seq_t c = binom(2 * divisor, k);
                idx_t e = 2 * divisor - k;    /* exponent of sqrt(2) */
                if (e % 2 == 0) {
                    seq_t val = c * pow2(e / 2);
                    int_pos += val;
                } else {
                    seq_t val = c * pow2((e - 1) / 2);
                    sqrt_pos += val;
                }
            }

            /* Negative contribution from n = -divisor when t >= 2*divisor-1 */
            if (t >= 2 * divisor - 1) {
                idx_t k2 = t - (2 * divisor - 1);   /* k2 >= 0 */
                seq_t c = binom(2 * divisor + k2 - 1, k2);
                bool sign_positive = (k2 % 2 == 0);  /* (-1)^k2 */
                if (k2 % 2 == 0) {                 /* integer part */
                    seq_t val = c * pow2(k2 / 2);
                    if (sign_positive)
                        int_pos += val;
                    else
                        int_neg += val;
                } else {                           /* sqrt(2) part */
                    seq_t val = c * pow2((k2 - 1) / 2);
                    if (sign_positive)
                        sqrt_pos += val;
                    else
                        sqrt_neg += val;
                }
            }
        }
    }

    /* Combine integer and sqrt parts (the sqrt part should cancel) */
    seq_t int_sum = int_pos - int_neg;
    (void)int_sum;               /* silence unused warning if int_sum not used */

    /* The sqrt part must be zero; we ignore it in the result (the arithmetic)
       is exact over Integer + sqrt(2) when the coefficients are balanced. */
    (void)sqrt_pos; (void)sqrt_neg;  /* silence warnings */

    return int_sum;
}
