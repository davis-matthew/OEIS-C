/*
 * A252770  Numbers k such that the heptagonal number H(k) is equal to the sum
 *          of the pentagonal numbers P(m), P(m+1), P(m+2) and P(m+3) for some
 *          m.
 * https://oeis.org/A252770
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 63*a(n-1) - 63*a(n-2) + a(n-3) initial values: a(1)=148,
 *            a(2)=9155, a(3)=567444
 * Range:     u8 n <= 1, u16 n <= 2, u32 n <= 5, u64 n <= 10, u128 n <= 21,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 0)            /* not part of OEIS but keeps function safe */
        return 0;

    if (n == 1)            /* a(1) */
        return (seq_t)148;
    if (n == 2)            /* a(2) */
        return (seq_t)9155;
    if (n == 3)            /* a(3) */
        return (seq_t)567444;

    /* Iterate the recurrence for n >= 4 */
    seq_t a1 = (seq_t)148;      /* a(n-3) */
    seq_t a2 = (seq_t)9155;     /* a(n-2) */
    seq_t a3 = (seq_t)567444;   /* a(n-1) */
    seq_t next;

    for (idx_t i = 4; i <= n; ++i) {
        next = (seq_t)63 * a3 - (seq_t)63 * a2 + a1;  /* a(i) */
        a1 = a2;
        a2 = a3;
        a3 = next;
    }
    return a3;
}
