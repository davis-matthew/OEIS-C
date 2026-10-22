/*
 * A325909  Lexicographically earliest sequence of distinct positive terms
 *          such that for any n > 0, n divides Sum_{k = 1..n} (-1)^k * a(k).
 * https://oeis.org/A325909
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(3k) = 3k - 1, a(3k+1) = 3k + 1, a(3k+2) = 6k + 3
 * Range:     u8 n <= 66, u16 n <= 66, u32 n <= 66, u64 n <= 66, u128 n <= 66,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 0) return 0;           /* offset starts at 1; undefined for n=0 */
    seq_t sn = (seq_t)n;
    if (sn % 3 == 0) {
        seq_t k = sn / 3;
        return 3*k - 1;
    } else if (sn % 3 == 1) {
        seq_t k = (sn - 1) / 3;
        return 3*k + 1;
    } else { /* sn % 3 == 2 */
        seq_t k = (sn - 2) / 3;
        return 6*k + 3;
    }
}
