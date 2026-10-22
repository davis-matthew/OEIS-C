/*
 * A394492  Expansion of e.g.f. (1/x) * Series_Reversion( x*(1 - 3*x)^(1/3) ).
 * https://oeis.org/A394492
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = (∏_{k=0}^{n-1} (4*n - 2 - 3*k)) / (n+1)
 * Range:     u8 n <= 2, u16 n <= 4, u32 n <= 7, u64 n <= 13, u128 n <= 22,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t prod = 1;
    idx_t i;
    for (i = 0; i < n; ++i) {
        seq_t factor = ((seq_t)4 * (seq_t)n) - (seq_t)2 - ((seq_t)3 * (seq_t)i);
        prod *= factor;
    }
    return prod / ((seq_t)n + (seq_t)1);
}
