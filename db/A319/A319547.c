/*
 * A319547  a(n) = 1*2*3*4*5*6*7 - 8*9*10*11*12*13*14 + 15*16*17*18*19*20*21 -
 *          ... + (up to n).
 * https://oeis.org/A319547
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  sign, easy
 * Formula:   a(n) = Σ_{j=1}^{⌈n/7⌉} (-1)^{j-1} * ∏_{i=(j-1)*7+1}^{min(j*7,n)}
 *            i
 * Range:     i8 n <= 5, i16 n <= 11, i32 n <= 27, i64 n <= 29, i128 n <= 29,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t sum = 0;
    seq_t product = 1;
    seq_t sign = 1; /* +1 for the first block, then alternates */
    for (idx_t i = 1; i <= n; ++i) {
        product *= (seq_t)i;
        if ((i % 7) == 0 || i == n) {
            sum += sign * product;
            sign = -sign;   /* flip sign for next block */
            product = 1;    /* reset product for next block */
        }
    }
    return sum;
}
