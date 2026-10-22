/*
 * A377501  a(n) = 2 + 4^(n - 1) - (2 - sqrt(2))^(n - 1) - (2 + sqrt(2))^(n -
 *          1).
 * https://oeis.org/A377501
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 2 + 4^(n-1) - (2 - sqrt(2))^(n-1) - (2 + sqrt(2))^(n-1)
 *            Recurrence: a(1)=1, a(2)=2, a(3)=6, a(4)=26
 *            a(n)=9*a(n-1)-26*a(n-2)+26*a(n-3)-8*a(n-4) for n≥5
 * Range:     u8 n <= 5, u16 n <= 9, u32 n <= 17, u64 n <= 26, u128 n <= 26,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 1) return (seq_t)1;
    if (n == 2) return (seq_t)2;
    if (n == 3) return (seq_t)6;
    if (n == 4) return (seq_t)26;

    seq_t t1 = (seq_t)1;   /* a(n-4) */
    seq_t t2 = (seq_t)2;   /* a(n-3) */
    seq_t t3 = (seq_t)6;   /* a(n-2) */
    seq_t t4 = (seq_t)26;  /* a(n-1) */
    seq_t next;

    for (idx_t i = 5; i <= n; ++i) {
        next = (9 * t4) - (26 * t3) + (26 * t2) - (8 * t1);
        t1 = t2;
        t2 = t3;
        t3 = t4;
        t4 = next;
    }
    return t4;
}
