/*
 * A309492  a(1) = a(2) = 1, a(3) = 3, a(4) = 5, a(5) = 2; a(n) = a(n-a(n-2))
 *          + a(n-a(n-3)) for n > 5.
 * https://oeis.org/A309492
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(1) = 1, a(2) = 1, a(3) = 3, a(4) = 5, a(5) = 2; For n > 5 :
 *            a(n) = a(n-a(n-2)) + a(n-a(n-3)), which simplifies to: a(4*k) =
 *            4*k+1, a(4*k+1) = 4*k-2, a(4*k+2) = 4, a(4*k+3) = 3
 * Range:     u8 n <= 255, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n) {
    /* Handle initial terms that fall outside the closed form */
    if (n <= 2)   return (seq_t)1;
    if (n == 3)   return (seq_t)3;
    if (n == 4)   return (seq_t)5;
    if (n == 5)   return (seq_t)2;

    /* n >= 6, use the simple periodic formula */
    idx_t k = n / 4;
    int     r = n % 4;   /* remainder 0..3 */

    switch (r) {
        case 0: /* n = 4*k   -> 4*k+1 */
            return (seq_t)4 * (seq_t)k + (seq_t)1;
        case 1: /* n = 4*k+1 -> 4*k-2 */
            return (seq_t)4 * (seq_t)k - (seq_t)2;
        case 2: /* n = 4*k+2 -> 4   */
            return (seq_t)4;
        case 3: /* n = 4*k+3 -> 3   */
            return (seq_t)3;
        default:
            /* This case is unreachable but keeps the compiler happy */
            return (seq_t)0;
    }
}
