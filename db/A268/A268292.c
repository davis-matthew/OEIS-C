/*
 * A268292  a(n) is the total number of isolated 1's at the boundary between
 *          n-th and (n-1)-th iterations in the pattern of A267489.
 * https://oeis.org/A268292
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  base, nonn
 * Formula:   a(n) = floor((n-3)^2/12) + floor((n-4)^2/12) for n > 0, a(0) = 0
 * Range:     u8 n <= 18, u16 n <= 55, u32 n <= 55, u64 n <= 55, u128 n <= 55,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 0)
        return 0;

    /* absolute differences */
    idx_t diff1 = (n >= 3) ? (n - 3) : (3 - n);
    idx_t diff2 = (n >= 4) ? (n - 4) : (4 - n);

    seq_t d1 = (seq_t)diff1;
    seq_t d2 = (seq_t)diff2;
    seq_t sq1 = d1 * d1;
    seq_t sq2 = d2 * d2;

    seq_t term1 = sq1 / 12;
    seq_t term2 = sq2 / 12;

    return term1 + term2;
}
