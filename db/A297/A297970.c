/*
 * A297970  Numbers that are not the sum of 3 squares and a nonnegative 7th
 *          power.
 * https://oeis.org/A297970
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, fini, full
 * Formula:   a(n) = 128*n - 16 for 1 ≤ n ≤ 17; 0 otherwise
 * Range:     u8 n <= 2, u16 n <= 17, u32 n <= 17, u64 n <= 17, u128 n <= 17,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A297970: Numbers that are not the sum of 3 squares and a nonnegative 7th power. */
seq_t a(idx_t n)
{
    if (n == 0)
        return 0;
    if (n <= (idx_t)17)
        return ((seq_t)128 * n) - (seq_t)16;
    return 0;
}
