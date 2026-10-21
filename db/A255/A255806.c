/*
 * A255806  Expansion of e.g.f.: exp(Sum_{k>=1} 3*x^k).
 * https://oeis.org/A255806
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = (2*n+1)*a(n-1) - (n-2)*(n-1)*a(n-2) (with a(0)=1, a(1)=3)
 * Range:     u8 n <= 3, u16 n <= 5, u32 n <= 10, u64 n <= 17, u128 n <= 30,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n) {
    if (n == 0) return (seq_t)1;
    if (n == 1) return (seq_t)3;
    seq_t a0 = 1;          /* a(0) */
    seq_t a1 = 3;          /* a(1) */
    seq_t ai;
    for (idx_t i = 2; i <= n; ++i) {
        ai = (((seq_t)2 * (seq_t)i + (seq_t)1) * a1)
             - (((seq_t)i - (seq_t)2) * ((seq_t)i - (seq_t)1) * a0);
        a0 = a1;
        a1 = ai;
    }
    return a1;
}
