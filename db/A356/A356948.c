/*
 * A356948  Sequence of scores adding to maximum break in snooker.
 * https://oeis.org/A356948
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, fini, full, easy
 * Formula:   a(n) = 1 if n odd and n ≤ 30; * 7 if n even and n ≤ 30; * 2 +
 *            (n−31) for 31 ≤ n ≤ 36; * 0 otherwise
 * Range:     u8 n <= 36, u16 n <= 36, u32 n <= 36, u64 n <= 36, u128 n <= 36,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n <= 30) {
        /* Alternating 1 and 7 for the 15 reds and 15 blacks. */
        return (n & 1) ? 1 : 7;
    }
    if (n >= 31 && n <= 36) {
        /* Final six colors: yellow (2), green (3), brown (4), blue (5), pink (6), black (7). */
        return 2 + (seq_t)(n - 31);
    }
    /* No further balls in a maximum break. */
    return 0;
}
