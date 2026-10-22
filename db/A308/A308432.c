/*
 * A308432  Given n cards in a stack numbered from 1 to n with 1 at the top,
 *          repeat the following process: first remove the card that is in the
 *          middle (at position (size of the stack)/2, rounding up), then move
 *          the card that is at the bottom of the stack to the top. This
 *          process is repeated until there is on...
 * https://oeis.org/A308432
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 1 if n is a power of 3; * t = 3^⌊log₃(n)⌋ * a(n) = t + 1
 *            if t < n ≤ 2t * a(n) = 3t − n + 1 otherwise (n > 2t)
 * Range:     u8 n <= 70, u16 n <= 70, u32 n <= 70, u64 n <= 70, u128 n <= 70,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Implements the recurrence described in OEIS A308432 */

seq_t a(idx_t n) {
    /* Find largest power of 3 not exceeding n */
    idx_t t = 1;
    while (t <= n / 3) {
        t *= 3;
    }

    if (t == n) {
        return (seq_t)1;            /* n is a power of 3 */
    }
    /* Check if n is in (t, 2t] without overflow */
    if (n - t <= t) {
        return (seq_t)(t + 1);      /* t < n ≤ 2t */
    }

    /* n > 2t:  a(n) = t - (n - 2t) + 1 */
    idx_t m = n - (t + t);          /* m = n - 2t  (1 ≤ m ≤ t-1) */
    return (seq_t)(t - m + 1);
}
