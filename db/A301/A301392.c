/*
 * A301392  a(n) = Product_{k=1..L} hypergeom([-n, -n], [1], k) with L = 3.
 * https://oeis.org/A301392
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = ∏_{k=1..3} _2F_1(-n,-n;1;k) = ∏_{k=1..3} Σ_{m=0..n}
 *            binom(n,m)^2 * k^m
 * Range:     u8 n <= 1, u16 n <= 2, u32 n <= 5, u64 n <= 9, u128 n <= 14,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t product = 1;
    for (unsigned int k = 1; k <= 3; ++k) {
        seq_t sum = 0;
        seq_t binom = 1;          /* binom(n,0) */
        seq_t pow_k = 1;          /* k^0 */
        for (idx_t m = 0; m <= n; ++m) {
            seq_t term = binom * binom;   /* binom(n,m)^2 */
            term = term * pow_k;         /* multiply by k^m */
            sum += term;

            if (m < n) {                           /* prepare next binomial and power */
                pow_k *= (seq_t)k;
                binom = binom * (seq_t)(n - m) / (m + 1);
            }
        }
        product *= sum;
    }
    return product;
}
