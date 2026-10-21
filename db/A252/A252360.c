/*
 * A252360  Numbers n such that the pentagonal number P(n) is equal to the sum
 *          of the heptagonal numbers H(m) and H(m+1) for some m.
 * https://oeis.org/A252360
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(1) = 289, a(2) = 139217, a(3) = 67102225, a(n) = 483*a(n-1) -
 *            483*a(n-2) + a(n-3) for n >= 4
 * Range:     u8 none, u16 n <= 1, u32 n <= 3, u64 n <= 7, u128 n <= 14,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0;            /* undefined, safety */
    if (n == 1) return (seq_t)289;
    if (n == 2) return (seq_t)139217;
    if (n == 3) return (seq_t)67102225;

    seq_t a1 = (seq_t)289;
    seq_t a2 = (seq_t)139217;
    seq_t a3 = (seq_t)67102225;

    for (idx_t i = 4; i <= n; ++i) {
        seq_t an = (seq_t)483 * a3 - (seq_t)483 * a2 + a1;
        a1 = a2;
        a2 = a3;
        a3 = an;
    }

    return a3;
}
