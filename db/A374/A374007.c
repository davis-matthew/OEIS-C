/*
 * A374007  a(n) = (1 + (n+5)^2 - (n-6)*(-1)^n)/2.
 * https://oeis.org/A374007
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = (1 + (n+5)^2 - (n-6)*(-1)^n)/2
 * Range:     u8 n <= 10, u16 n <= 52, u32 n <= 52, u64 n <= 52, u128 n <= 52,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* n >= 1; the sequence starts at offset 1. */
    if (n & 1) {                         /* n is odd */
        seq_t np5 = (seq_t)n + 5;        /* (n+5) */
        seq_t term = np5 * np5 + (seq_t)n - 5;   /* (n+5)^2 + n - 5 */
        return term / 2;
    } else {                            /* n is even */
        seq_t np5 = (seq_t)n + 5;        /* (n+5) */
        seq_t term = np5 * np5 - (seq_t)n + 7;   /* (n+5)^2 - n + 7 */
        return term / 2;
    }
}
