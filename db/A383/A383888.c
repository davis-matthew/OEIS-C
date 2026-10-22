/*
 * A383888  a(n) = Sum_{k=0..n} 3^k * binomial(n+k-1,k).
 * https://oeis.org/A383888
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Sum_{k=0..n} binomial(n+k-1,k) * 3^k
 * Range:     u8 n <= 2, u16 n <= 5, u32 n <= 9, u64 n <= 18, u128 n <= 36,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    idx_t k;
    seq_t a_val = 0;          /* accumulator for the sum               */
    seq_t binom = 1;          /* binomial(n+k-1, k), starting at k=0    */
    seq_t pow3  = 1;          /* 3^k, starts at k=0                     */

    for (k = 0; k <= n; ++k) {
        a_val += binom * pow3;
        /* update binomial for next k:  binom(n+k, k+1) */
        binom = binom * ((seq_t)n + (seq_t)k) / ((seq_t)k + 1);
        pow3  = pow3 * 3;
    }
    return a_val;
}
