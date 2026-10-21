/*
 * A250256  Least positive integer whose decimal digits divide the plane into
 *          n regions (A249572 variant).
 * https://oeis.org/A250256
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base, easy
 * Formula:   not stated
 * Range:     u8 n <= 5, u16 n <= 9, u32 n <= 19, u64 n <= 28, u128 n <= 28,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence: A250256
 * Formula: a(n) = 10*a(n-2)+8  (n >= 4)
 * Initial values: a(1)=1, a(2)=6, a(3)=8
 */

seq_t a(idx_t n)
{
    /* Handle initial terms directly. */
    if (n == 1) return (seq_t)1;
    if (n == 2) return (seq_t)6;
    if (n == 3) return (seq_t)8;

    /* Even indices: use the recurrence starting from a(2)=6. */
    if ((n & 1) == 0) {
        seq_t val = (seq_t)6;          /* a(2) */
        for (idx_t i = 4; i <= n; i += 2) {
            val = val * 10 + 8;        /* a(i) = 10*a(i-2)+8 */
        }
        return val;
    }

    /* Odd indices (n >= 5): start from a(3)=8. */
    seq_t val = (seq_t)8;              /* a(3) */
    for (idx_t i = 5; i <= n; i += 2) {
        val = val * 10 + 8;            /* a(i) = 10*a(i-2)+8 */
    }
    return val;
}
