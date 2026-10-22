/*
 * A363022  Expansion of Sum_{k>0} x^(2*k)/(1+x^k)^3.
 * https://oeis.org/A363022
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  sign
 * Formula:   a(n) = Σ_{d|n} (-1)^d * binomial(d,2)
 * Range:     i8 n <= 15, i16 n <= 215, i32 n <= 10000, i64 n <= 10000,
 *            i128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t sum = 0;
    for (idx_t i = 1; i <= n / i; ++i) {
        if (n % i == 0) {
            /* divisor d = i */
            seq_t d = (seq_t)i;
            seq_t binom = (d * (d - 1)) / 2;   /* binomial(d,2) */
            if ((i & 1) == 0)
                sum += binom;
            else
                sum -= binom;

            /* divisor d = n / i, if distinct */
            idx_t j = n / i;
            if (j != i) {
                seq_t d2 = (seq_t)j;
                seq_t binom2 = (d2 * (d2 - 1)) / 2;
                if ((j & 1) == 0)
                    sum += binom2;
                else
                    sum -= binom2;
            }
        }
    }
    return sum;
}
