/*
 * A344805  Numbers that are the sum of six squares in one or more ways.
 * https://oeis.org/A344805
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = (n > 7) ? n + 12 : a(1) = 6, a(2) = 9, a(3) = 12, a(4) =
 *            14, a(5) = 15, a(6) = 17, a(7) = 18
 * Range:     u8 n <= 243, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n <= 7) {
        /* the first 7 terms are hard‑coded */
        static const seq_t first[7] = {
            (seq_t)6, (seq_t)9, (seq_t)12,
            (seq_t)14, (seq_t)15, (seq_t)17,
            (seq_t)18
        };
        return first[n - 1];
    }
    /* all subsequent terms satisfy a(n) = n + 12 */
    return (seq_t)n + (seq_t)12;
}
