/*
 * A363570  Expansion of (1 / sqrt(1 - 8*x + 4*x^2) - 1 / (1 - x)) / 3.
 * https://oeis.org/A363570
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Sum_{k=1..n} binomial(n,k)^2 * 3^(k-1)
 * Range:     u8 n <= 3, u16 n <= 6, u32 n <= 12, u64 n <= 22, u128 n <= 22,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 0) {
        return 0;
    }
    seq_t binom = 1;   /* binomial(n,0) */
    seq_t pow3  = 1;   /* 3^(0)   */
    seq_t sum   = 0;

    for (idx_t k = 1; k <= n; ++k) {
        /* binom = binom * (n-k+1) / k  */
        binom = binom * (seq_t)(n - k + 1) / (seq_t)k;
        sum += binom * binom * pow3;
        pow3 *= 3;
    }
    return sum;
}
