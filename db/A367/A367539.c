/*
 * A367539  Sequence of magic constants related to distance magic graphs.
 * https://oeis.org/A367539
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = the n-th positive integer not equal to 1, 2, 4, 6, 8, 12,
 *            or 16
 * Range:     u8 n <= 68, u16 n <= 68, u32 n <= 68, u64 n <= 68, u128 n <= 68,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    idx_t count = 0;
    for (idx_t val = 1;; ++val)
    {
        if (val == 1 || val == 2 || val == 4 ||
            val == 6 || val == 8 || val == 12 || val == 16)
            continue;                 /* skip excluded numbers */
        ++count;
        if (count == n)
            return (seq_t)val;        /* found the n-th valid integer */
    }
}
