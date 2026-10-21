/*
 * A255821  Numbers of words on {0,1,...,36} having no isolated zeros.
 * https://oeis.org/A255821
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 37*a(n-1) - 36*a(n-2) + 36*a(n-3) Initial values: a(0)=1,
 *            a(1)=36, a(2)=1297
 * Range:     u8 n <= 1, u16 n <= 3, u32 n <= 6, u64 n <= 12, u128 n <= 24,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 0) return 1;
    if (n == 1) return 36;
    if (n == 2) return 1297;

    seq_t a0 = 1;      /* a(n-3) */
    seq_t a1 = 36;     /* a(n-2) */
    seq_t a2 = 1297;   /* a(n-1) */
    seq_t a3;

    for (idx_t i = 3; i <= n; ++i) {
        a3 = (seq_t)37 * a2 - (seq_t)36 * a1 + (seq_t)36 * a0;
        a0 = a1;
        a1 = a2;
        a2 = a3;
    }

    return a2;
}
