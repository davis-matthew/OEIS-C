/*
 * A254628  Triangular numbers (A000217) that are also centered pentagonal
 *          numbers (A005891).
 * https://oeis.org/A254628
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = a(n-1)+322*a(n-2)-322*a(n-3)-a(n-4)+a(n-5) Initial
 *            values: a(1)=1, a(2)=6, a(3)=276, a(4)=1891, a(5)=88831
 * Range:     u8 n <= 2, u16 n <= 4, u32 n <= 8, u64 n <= 16, u128 n <= 32,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* Offset handling – values for n < 1 are irrelevant */
    if (n == 0) return 0;

    /* Return predefined first five terms */
    if (n == 1) return 1;
    if (n == 2) return 6;
    if (n == 3) return 276;
    if (n == 4) return 1891;
    if (n == 5) return 88831;

    /* Keep only the last six values – no dynamic allocation */
    seq_t a1 = 1;          /* a(1) */
    seq_t a2 = 6;          /* a(2) */
    seq_t a3 = 276;        /* a(3) */
    seq_t a4 = 1891;       /* a(4) */
    seq_t a5 = 88831;      /* a(5) */
    seq_t an;              /* current term */

    for (idx_t i = 6; i <= n; ++i) {
        an = a5
            + (seq_t)322 * a4
            - (seq_t)322 * a3
            - a2
            + a1;

        /* Shift terms for next iteration */
        a1 = a2;
        a2 = a3;
        a3 = a4;
        a4 = a5;
        a5 = an;
    }

    return an;
}
