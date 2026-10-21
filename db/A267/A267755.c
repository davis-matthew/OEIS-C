/*
 * A267755  Expansion of (1 + 2*x + x^2 + x^3 + 4*x^4 + 2*x^5)/(1 - x - x^5 +
 *          x^6).
 * https://oeis.org/A267755
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = n+1 + 2*floor(n/5) + 3*floor((n+1)/5) + floor((n+4)/5)
 * Range:     u8 n <= 115, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t N = (seq_t)n;
    seq_t t = N + (seq_t)1;
    t += (seq_t)2 * (N / (seq_t)5);
    t += (seq_t)3 * ((N + (seq_t)1) / (seq_t)5);
    t += (N + (seq_t)4) / (seq_t)5;
    return t;
}
