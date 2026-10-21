/*
 * A267017  Digital roots of the stella octangula numbers.
 * https://oeis.org/A267017
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, base, easy
 * Formula:   a(n) = digital root of A007588(n) Recurrence: a(n) = a(n-9) for
 *            n>9, a(0) = 0 (Verification: a(1)=1, a(2)=5, a(3)=6, a(4)=7,
 *            a(5)=2, a(6)=3, a(7)=4, a(8)=8, a(9)=9, a(10)=1, …)
 * Range:     u8 n <= 1000, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 0)
        return (seq_t)0;

    /* n mod 9 gives a number 0..8. We need the mapping:
       r=1→1, 2→5, 3→6, 4→7, 5→2, 6→3, 7→4, 8→8, 0→9   */
    idx_t r = n % 9;
    if (r == 0)
        return (seq_t)9;

    static const seq_t tbl[8] = {
        (seq_t)1, /* r=1 */
        (seq_t)5, /* r=2 */
        (seq_t)6, /* r=3 */
        (seq_t)7, /* r=4 */
        (seq_t)2, /* r=5 */
        (seq_t)3, /* r=6 */
        (seq_t)4, /* r=7 */
        (seq_t)8  /* r=8 */
    };
    return tbl[r - 1];
}
