/*
 * A286748  Characteristic sequence of the Beatty sequence, A194028, of
 *          sqrt(12).
 * https://oeis.org/A286748
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = floor((n+1)/sqrt(12)) - floor(n/sqrt(12))
 * Range:     u8 n <= 10000, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* 1/sqrt(12) ≈ 0.28867513459481287 */
    static const double inv_alpha = 0.28867513459481287;
    idx_t f1 = (idx_t)((double)(n + 1) * inv_alpha);
    idx_t f0 = (idx_t)((double)n * inv_alpha);
    return (seq_t)(f1 > f0 ? 1 : 0);
}
