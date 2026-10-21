/*
 * A268742  Expansion of x*(1 + x + 18*x^2 + 10*x^3 - x^4 + 11*x^5 + 18*x^6 +
 *          2*x^7) / (1 - x - 2*x^4 + 2*x^5 + x^8 - x^9).
 * https://oeis.org/A268742
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(4*k) = 30*k^2; a(4*k+1) = 30*k^2 + 1; a(4*k+2) = 30*k^2 + 12*k
 *            + 2; a(4*k+3) = 30*k^2 + 48*k + 20 (k = n/4, r = n%4)
 * Range:     u8 n <= 11, u16 n <= 186, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    idx_t k = n / 4;
    seq_t sq = (seq_t)k * (seq_t)k;          /* k^2 */
    seq_t base = (seq_t)30 * sq;             /* 30*k^2 */
    idx_t r = n % 4;
    switch (r) {
        case 0: return base;
        case 1: return base + (seq_t)1;
        case 2: return base + (seq_t)12 * (seq_t)k + (seq_t)2;
        case 3: return base + (seq_t)48 * (seq_t)k + (seq_t)20;
    }
    /* This point is never reached, but return a sane value. */
    return base;
}
