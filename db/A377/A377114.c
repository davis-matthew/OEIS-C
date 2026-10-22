/*
 * A377114  a(n) = coefficient of sqrt(2) in the expansion of (3 + sqrt(2) +
 *          sqrt(3))^n.
 * https://oeis.org/A377114
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = coefficient of sqrt(2) in (3 + sqrt(2) + sqrt(3))^n. *
 *            Recurrence: a(0)=0, a(1)=1, a(2)=6, a(3)=38, * a(n) = 12*a(n-1)
 *            - 44*a(n-2) + 48*a(n-3) + 8*a(n-4) for n >= 4
 * Range:     u8 n <= 4, u16 n <= 7, u32 n <= 13, u64 n <= 22, u128 n <= 22,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 0) return 0;
    if (n == 1) return 1;
    if (n == 2) return 6;
    if (n == 3) return 38;

    seq_t a0 = 0, a1 = 1, a2 = 6, a3 = 38; /* a(i-4), a(i-3), a(i-2), a(i-1)  */
    for (idx_t i = 4; i <= n; ++i) {
        seq_t next = (12 * a3 + 48 * a1 + 8 * a0) - 44 * a2;
        a0 = a1;
        a1 = a2;
        a2 = a3;
        a3 = next;
    }
    return a3;
}
