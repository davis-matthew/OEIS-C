/*
 * A290745  Maximum number of distinct Lyndon factors that can appear in words
 *          of length n over an alphabet of size 10.
 * https://oeis.org/A290745
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = binomial(n+1,2) - (s-p)*binomial(m+1,2) -
 *            p*binomial(m+2,2) + s, where s=10, m=floor(n/s), p=n-m*s
 * Range:     u8 n <= 23, u16 n <= 381, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* This formula is valid for n >= 1; it also works for n = 0. */
    const idx_t s = 10;
    idx_t m = n / s;
    idx_t p = n - m * s;

    seq_t nn   = (seq_t)n;
    seq_t bin_n = (nn + 1) * nn / 2;

    seq_t bin_m1 = (seq_t)(m + 1) * (seq_t)m / 2;
    seq_t bin_m2 = (seq_t)(m + 2) * (seq_t)(m + 1) / 2;

    seq_t term2 = (seq_t)(s - p) * bin_m1; /* contribution of the (s-p)*... term */
    seq_t term3 = (seq_t)p * bin_m2; /* contribution of the p*... term */

    return bin_n - term2 - term3 + (seq_t)s;
}
