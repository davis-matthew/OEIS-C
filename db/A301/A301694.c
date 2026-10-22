/*
 * A301694  Expansion of (1 + 5*x + 4*x^2 + 5*x^3 + x^4)/((1 - x)*(1 - x^3)).
 * https://oeis.org/A301694
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(0) = 1; for n > 0, a(n) = 6*n - 2*floor((n+1)/3)
 * Range:     u8 n <= 47, u16 n <= 5000, u32 n <= 5000, u64 n <= 5000,
 *            u128 n <= 5000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 0)
        return (seq_t)1;
    seq_t n_seq = (seq_t)n;
    seq_t m = (seq_t)((n + 1) / 3);
    return 6 * n_seq - 2 * m;
}
