/*
 * A336428  Total number of graceful labelings of the complement of the path
 *          graph P_n.
 * https://oeis.org/A336428
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, more
 * Formula:   a(n) = 0 for n >= 10; * a(1) = 1; * a(n) = 4 * A387827(n) for 2
 *            <= n <= 9, * where the values of 4*A387827(n) for these n are *
 *            0, 0, 4, 52, 136, 180, 72, 4 respectively
 * Range:     u8 n <= 85, u16 n <= 85, u32 n <= 85, u64 n <= 85, u128 n <= 85,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n < 1) return (seq_t)0;          /* undefined index */
    if (n == 1) return (seq_t)1;          /* a(1) = 1 */
    if (n >= 10) return (seq_t)0;          /* a(n) = 0 for n >= 10 */

    /* 2 <= n <= 9: use the exact values */
    switch ((idx_t)n)
    {
        case 2: return (seq_t)0;
        case 3: return (seq_t)0;
        case 4: return (seq_t)4;
        case 5: return (seq_t)52;
        case 6: return (seq_t)136;
        case 7: return (seq_t)180;
        case 8: return (seq_t)72;
        case 9: return (seq_t)4;
        default: return (seq_t)0;  /* unreachable, but keeps the compiler happy */
    }
}
