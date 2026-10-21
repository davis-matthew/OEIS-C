/*
 * A268633  Number of n X 2 0..2 arrays with some element plus some
 *          horizontally or vertically adjacent neighbor totalling two exactly
 *          once.
 * https://oeis.org/A268633
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 8 * 3^(n-2) * (2*n-1) for n>1, a(1)=3
 * Range:     u8 n <= 3, u16 n <= 7, u32 n <= 17, u64 n <= 36, u128 n <= 76,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n < 2) {
        return (seq_t)3;
    }

    idx_t exp = n - 2;
    seq_t power = (seq_t)1;
    for (idx_t i = 0; i < exp; ++i) {
        power *= (seq_t)3;
    }

    seq_t mult = (seq_t)8 * (seq_t)(2 * n - 1);
    return power * mult;
}
