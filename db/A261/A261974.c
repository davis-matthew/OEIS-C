/*
 * A261974  The first of eleven consecutive positive integers the sum of the
 *          squares of which is equal to the sum of the squares of three
 *          consecutive positive integers.
 * https://oeis.org/A261974
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 47*a(n-1) - 47*a(n-2) + a(n-3), a(1)=67, a(2)=3307,
 *            a(3)=152275
 * Range:     u8 n <= 1, u16 n <= 2, u32 n <= 5, u64 n <= 11, u128 n <= 23,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n) {
    if (n == 0) return 0;            /* undefined for n=0, return 0 */
    if (n == 1) return 67;
    if (n == 2) return 3307;
    if (n == 3) return 152275;

    seq_t a1 = 67;                   /* a(n-3) */
    seq_t a2 = 3307;                 /* a(n-2) */
    seq_t a3 = 152275;               /* a(n-1) */
    const seq_t coeff = 47;

    for (idx_t i = 4; i <= n; ++i) {
        seq_t cur = coeff * a3 - coeff * a2 + a1;  /* a(i) */
        a1 = a2;
        a2 = a3;
        a3 = cur;
    }
    return a3;
}
