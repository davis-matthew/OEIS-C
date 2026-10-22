/*
 * A392885  a(n) is the maximum total surface area of four-element sets of
 *          distinct integer-sided cuboids that fill an n X n X n cube.
 * https://oeis.org/A392885
 *
 * Offset:    3    (a(3) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 12*n^2 for n >= 10; for 3 <= n <= 9 the values are *
 *            pre‑computed as they appear in the OEIS sequence. For n < 3 the
 *            * function returns 0 as the sequence is undefined below its
 *            offset
 * Range:     u8 n <= 4, u16 n <= 45, u32 n <= 45, u64 n <= 45, u128 n <= 45,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* handle the undefined range */
    if (n < 3) return 0;

    /* values for n = 3..9 */
    static const seq_t small[] = {
        /* indices 0..9 */
        0, 0, 0,
        92,   /* n = 3 */
        170,  /* n = 4 */
        272,  /* n = 5 */
        408,  /* n = 6 */
        560,  /* n = 7 */
        736,  /* n = 8 */
        936    /* n = 9 */
    };

    if (n < 10) return small[n];

    /* n >= 10: use the simple quadratic formula */
    return ((seq_t)n * (seq_t)n) * 12;
}
