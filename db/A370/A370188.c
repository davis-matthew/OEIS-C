/*
 * A370188  a(n) = sqrt((A114052(n)^2 - 1)/27)/5.
 * https://oeis.org/A370188
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 2, u16 n <= 3, u32 n <= 6, u64 n <= 12, u128 n <= 23,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Recurrence: a(0) = 0, a(1) = 1, a(n) = 52*a(n-1) - a(n-2) for n >= 2 */

seq_t a(idx_t n)
{
    /* Handle the initial two terms */
    if (n == 0) return (seq_t)0;
    if (n == 1) return (seq_t)1;

    /* Iterate using O(n) time and O(1) memory */
    seq_t a_prevprev = (seq_t)0;   /* a(0) */
    seq_t a_prev     = (seq_t)1;   /* a(1) */
    seq_t a_current  = 0;

    for (idx_t i = 2; i <= n; ++i) {
        a_current = (seq_t)52 * a_prev - a_prevprev;
        a_prevprev = a_prev;
        a_prev = a_current;
    }

    return a_current;
}
