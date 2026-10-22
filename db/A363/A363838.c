/*
 * A363838  a(n) = Product_{b=2..n} b^gamma(n, b) where gamma(n, b) =
 *          Sum_{i>=1} floor(n/b^i).
 * https://oeis.org/A363838
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Product_{b=2..n} b^gamma(n,b) where * gamma(n,b) =
 *            Sum_{i>=1} floor(n / b^i)
 * Range:     u8 n <= 4, u16 n <= 6, u32 n <= 9, u64 n <= 13, u128 n <= 20,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t result = 1;

    for (idx_t b = 2; b <= n; ++b) {
        /* Compute gamma(n,b) = sum_{i>=1} floor(n/b^i) */
        idx_t gamma = 0;
        idx_t div   = n / b;          /* first floor(n/b) */
        while (div > 0) {
            gamma += div;
            div   /= b;               /* floor( floor(n/b^i)/b ) = floor(n/b^(i+1)) */
        }

        /* Compute b^gamma  (gamma may be 0) */
        seq_t term = 1;
        for (idx_t e = 0; e < gamma; ++e) {
            term *= (seq_t)b;
        }

        result *= term;
    }

    return result;
}
