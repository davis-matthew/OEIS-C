/*
 * A254652  Indices of pentagonal numbers (A000326) which are also centered
 *          heptagonal numbers (A069099).
 * https://oeis.org/A254652
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = a(n-1)+110*a(n-2)-110*a(n-3)-a(n-4)+a(n-5) Initial
 *            values: a(1)=1, a(2)=4, a(3)=88, a(4)=421, a(5)=9661
 * Range:     u8 n <= 3, u16 n <= 6, u32 n <= 10, u64 n <= 20, u128 n <= 38,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n) {
    if (n == 1) return (seq_t)1;
    if (n == 2) return (seq_t)4;
    if (n == 3) return (seq_t)88;
    if (n == 4) return (seq_t)421;
    if (n == 5) return (seq_t)9661;

    seq_t a1 = (seq_t)1,   /* a(n-5)   */
          a2 = (seq_t)4,   /* a(n-4)   */
          a3 = (seq_t)88,  /* a(n-3)   */
          a4 = (seq_t)421, /* a(n-2)   */
          a5 = (seq_t)9661;/* a(n-1)   */

    seq_t ai;
    for (idx_t i = 6; i <= n; ++i) {
        ai = a5 + (seq_t)110 * a4 - (seq_t)110 * a3 - a2 + a1;
        a1 = a2;
        a2 = a3;
        a3 = a4;
        a4 = a5;
        a5 = ai;
    }
    return a5;
}
