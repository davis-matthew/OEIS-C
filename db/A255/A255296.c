/*
 * A255296  a(n) = A255295(2^n-1).
 * https://oeis.org/A255296
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 5*a(n-1) - 4*a(n-2) - 4*a(n-3) initial values: a(0)=1,
 *            a(1)=6, a(2)=24, for n>2 This implements OEIS A255296 as
 *            a(n)=A255295(2^n-1)
 * Range:     u8 n <= 3, u16 n <= 8, u32 n <= 16, u64 n <= 34, u128 n <= 69,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 0) return 1;
    if (n == 1) return 6;
    if (n == 2) return 24;
    seq_t a0 = 1, a1 = 6, a2 = 24, a3;
    for (idx_t i = 3; i <= n; ++i) {
        /* compute next term: a3 = 5*a2 - 4*a1 - 4*a0 */
        a3 = 5*a2 - 4*a1 - 4*a0;
        a0 = a1;
        a1 = a2;
        a2 = a3;
    }
    return a2;
}
