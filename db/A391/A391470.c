/*
 * A391470  Number of words of length 3*n that can be formed with a
 *          three-letter alphabet when the number of letters of each type is
 *          == 2 (mod 3).
 * https://oeis.org/A391470
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 27*(a(n-1) - a(n-2) + 27*a(n-3)); a(0)=0, a(1)=0,
 *            a(2)=90, a(3)=2268
 * Range:     u8 n <= 2, u16 n <= 4, u32 n <= 7, u64 n <= 14, u128 n <= 27,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0;
    if (n == 1) return (seq_t)0;
    if (n == 2) return (seq_t)90;
    if (n == 3) return (seq_t)2268;

    seq_t a0 = (seq_t)0;     /* a(n-3) */
    seq_t a1 = (seq_t)90;    /* a(n-2) */
    seq_t a2 = (seq_t)2268;  /* a(n-1) */
    seq_t curr = 0;

    for (idx_t i = 4; i <= n; ++i) {
        curr = (seq_t)27 * (a2 - a1 + (seq_t)27 * a0);
        a0 = a1;
        a1 = a2;
        a2 = curr;
    }

    return curr;
}
