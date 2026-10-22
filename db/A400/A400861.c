/*
 * A400861  a(n) is the largest N such that the integers 1..n can be arranged
 *          in a row so that the sums of blocks of adjacent terms (including
 *          single terms) cover every integer from 1 to N.
 * https://oeis.org/A400861
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, more, hard, new
 * Formula:   a(n) = 0,1,3,6,8,11,16,21,26,32,37 for n≤10 * a(n) = ⌊ n·(n+1)/3
 *            ⌋ for n≥11 * (OEIS: A400861 – maximum N such that 1..n can be
 *            permuted so that all * contiguous sums cover 1..N.)
 * Range:     u8 n <= 13, u16 n <= 13, u32 n <= 13, u64 n <= 13, u128 n <= 13,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* Explicit values for n ≤ 10 */
    static const seq_t small[11] = {
        0, 1, 3, 6, 8, 11, 16, 21, 26, 32, 37
    };
    if (n <= 10) {
        return small[(idx_t)n];
    }
    /* For larger n, a(n) = floor(n*(n+1)/3) */
    seq_t nn = (seq_t)n;
    return nn * (nn + (seq_t)1) / (seq_t)3;
}
