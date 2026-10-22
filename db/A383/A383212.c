/*
 * A383212  a(n) = permanent of the n-th principal submatrix of the
 *          rectangular array whose odd-numbered rows are
 *          (2,1,2,1,2,1,2,1,...) and even-numbered rows are
 *          (1,2,1,2,1,2,1,2,...).
 * https://oeis.org/A383212
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = (m!)^2 * Σ_{k=0..m} binomial(m,k)^2 * 4^k for n = 2*m
 *            a(n) = 2*m!*(m+1)! * Σ_{k=0..m}
 *            binomial(m,k)*binomial(m+1,k+1)*4^k for n = 2*m+1
 * Range:     u8 n <= 4, u16 n <= 6, u32 n <= 11, u64 n <= 18, u128 n <= 30,
 *            big any n
 * Helpers:   binomial, factorial
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/binomial.h"
#include "../utils/factorial.h"

seq_t a(idx_t n)
{
    /* Handle the zero case explicitly (works also with the even formula) */
    if (n == 0)
        return (seq_t)1;

    /* even n = 2*m */
    if ((n & 1) == 0) {
        idx_t m = n >> 1;
        seq_t fact_m = factorial(m);
        seq_t sum = 0;
        seq_t pow4 = 1;
        seq_t bin;

        for (idx_t k = 0; k <= m; ++k) {
            bin = binomial(m, k);
            sum += (bin * bin) * pow4;
            pow4 *= 4;
        }
        return fact_m * fact_m * sum;
    }

    /* odd n = 2*m+1 */
    idx_t m = (n - 1) >> 1;
    seq_t fact_m  = factorial(m);
    seq_t fact_mp1 = fact_m * (seq_t)(m + 1);   /* (m+1)! = m! * (m+1) */

    seq_t sum = 0;
    seq_t pow4 = 1;
    seq_t bin_mk, bin_mp1_k1, term;

    for (idx_t k = 0; k <= m; ++k) {
        bin_mk   = binomial(m, k);
        bin_mp1_k1 = binomial(m + 1, k + 1);
        term = bin_mk * bin_mp1_k1;
        term = term * pow4;
        sum += term;
        pow4 *= 4;
    }

    return 2 * fact_m * fact_mp1 * sum;
}
