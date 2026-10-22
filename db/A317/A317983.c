/*
 * A317983  Expansion of 420*x*(1 + x)*(1 + 10*x + x^2) / (1 - x)^6.
 * https://oeis.org/A317983
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 84*n^5 + 210*n^4 + 140*n^3 - 14*n
 * Range:     u8 none, u16 n <= 3, u32 n <= 34, u64 n <= 1000, u128 n <= 1000,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t N   = (seq_t)n;     /* n is 1‑based */
    seq_t n2  = N * N;
    seq_t n3  = n2 * N;
    seq_t n4  = n3 * N;
    seq_t term = n4 * (seq_t)84 + n3 * (seq_t)210 + n2 * (seq_t)140 - (seq_t)14;
    return N * term;
}
