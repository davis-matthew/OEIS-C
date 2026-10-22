/*
 * A343545  a(n) = n * Sum_{d|n} binomial(d+3,4)/d.
 * https://oeis.org/A343545
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = n * Sum_{d|n} binomial(d+3,4)/d
 * Range:     u8 n <= 3, u16 n <= 33, u32 n <= 44, u64 n <= 44, u128 n <= 44,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Equivalent: a(n) = n/24 * (σ3 + 6*σ2 + 11*σ1 + 6*σ0) where σk sums d^k over divisors of n */

seq_t a(idx_t n)
{
    if (n == 0)
        return 0;          /* undefined at offset 0, but safe for completeness */

    seq_t sigma0 = 0;     /* number of divisors */
    seq_t sigma1 = 0;     /* sum of divisors */
    seq_t sigma2 = 0;     /* sum of squares of divisors */
    seq_t sigma3 = 0;     /* sum of cubes of divisors */

    for (idx_t d = 1; d <= n / d; ++d) {        /* iterate over d, avoiding overflow */
        if (n % d == 0) {
            /* divisor d */
            seq_t dd = d;
            seq_t dd2 = dd * dd;
            seq_t dd3 = dd2 * dd;

            sigma0 += 1;
            sigma1 += dd;
            sigma2 += dd2;
            sigma3 += dd3;

            idx_t p = n / d;    /* paired divisor */
            if (p != d) {
                seq_t ddp = p;
                seq_t ddp2 = ddp * ddp;
                seq_t ddp3 = ddp2 * ddp;

                sigma0 += 1;
                sigma1 += ddp;
                sigma2 += ddp2;
                sigma3 += ddp3;
            }
        }
    }

    seq_t temp = sigma3 + (seq_t)6 * sigma2 + (seq_t)11 * sigma1 + (seq_t)6 * sigma0;
    seq_t result = (seq_t)n * temp / (seq_t)24;
    return result;
}
