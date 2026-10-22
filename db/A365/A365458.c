/*
 * A365458  The largest power of 3 that is less than or equal to n.
 * https://oeis.org/A365458
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 3^floor((log n) / (log 3))
 * Range:     u8 n <= 727, u16 n <= 9387, u32 n <= 9387, u64 n <= 9387,
 *            u128 n <= 9387, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* n is at least 1 for the offset 1 of the sequence. */
    seq_t res = 1;
    seq_t limit = (seq_t)n;

    /* Multiply by 3 while the next multiplication would stay ≤ n. */
    while (res <= limit / 3)
        res *= 3;

    return res;
}
