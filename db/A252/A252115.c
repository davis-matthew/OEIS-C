/*
 * A252115  Numbers n such that the sum of the heptagonal numbers H(n), H(n+1)
 *          and H(n+2) is equal to the pentagonal number P(m) for some m.
 * https://oeis.org/A252115
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) satisfies recurrence a(n) = a(n-1) + 103682*a(n-2) -
 *            103682*a(n-3) - a(n-4) + a(n-5) with initial values a(1)=573,
 *            a(2)=26951, a(3)=59482389, a(4)=2794406159, a(5)=6167253128301
 * Range:     u8 none, u16 n <= 2, u32 n <= 4, u64 n <= 7, u128 n <= 15,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n < 1) return (seq_t)0;

    /* 5 base terms */
    const seq_t v1 = (seq_t)573;
    const seq_t v2 = (seq_t)26951;
    const seq_t v3 = (seq_t)59482389;
    const seq_t v4 = (seq_t)2794406159;
    const seq_t v5 = (seq_t)6167253128301;

    if (n == 1) return v1;
    if (n == 2) return v2;
    if (n == 3) return v3;
    if (n == 4) return v4;
    if (n == 5) return v5;

    seq_t a1 = v1, a2 = v2, a3 = v3, a4 = v4, a5 = v5, next;

    for (idx_t i = 6; i <= n; ++i) {
        next = a5 + (seq_t)103682 * a4 - (seq_t)103682 * a3 - a2 + a1;
        a1 = a2;
        a2 = a3;
        a3 = a4;
        a4 = a5;
        a5 = next;
    }
    return a5;
}
