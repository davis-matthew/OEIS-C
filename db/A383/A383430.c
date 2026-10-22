/*
 * A383430  a(n) is the size of the largest subset of {1,...,n} such that no
 *          two elements differ by 5 or 8.
 * https://oeis.org/A383430
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 76, u16 n <= 76, u32 n <= 76, u64 n <= 76, u128 n <= 76,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Recurrence: a(n) = a(n-13) + 6   for n > 20
   Initial values: a(1)=1, a(2)=2, a(3)=3, a(4)=4, a(5)=5,
   a(6)=5, a(7)=5, a(8)=5, a(9)=5, a(10)=5,
   a(11)=6, a(12)=6, a(13)=6, a(14)=7, a(15)=8,
   a(16)=8, a(17)=9, a(18)=10, a(19)=10, a(20)=10
*/
seq_t a(idx_t n)
{
    /* base values for indices 1..20 (init[0] unused) */
    static const seq_t init[21] = {
        0,   /* 0th index, unused */
        1,   /* a(1)  */
        2,   /* a(2)  */
        3,   /* a(3)  */
        4,   /* a(4)  */
        5,   /* a(5)  */
        5,   /* a(6)  */
        5,   /* a(7)  */
        5,   /* a(8)  */
        5,   /* a(9)  */
        5,   /* a(10) */
        6,   /* a(11) */
        6,   /* a(12) */
        6,   /* a(13) */
        7,   /* a(14) */
        8,   /* a(15) */
        8,   /* a(16) */
        9,   /* a(17) */
        10,  /* a(18) */
        10,  /* a(19) */
        10   /* a(20) */
    };

    if (n <= 20)
        return init[n];

    /* number of recurrence applications needed to reduce n to ≤20 */
    idx_t k = 1 + (n - 21) / 13;          /* k >= 1 */
    idx_t m = n - 13 * k;                 /* 1 ≤ m ≤ 20 */

    return init[m] + (seq_t)6 * (seq_t)k;
}
