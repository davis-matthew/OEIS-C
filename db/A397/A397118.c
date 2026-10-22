/*
 * A397118  Size of clusters of star triples of Family D in the Spectre
 *          aperiodic monotile tiling.
 * https://oeis.org/A397118
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 4*a(n-1) + 12*a(n-2) - 14*a(n-3) - a(n-4) for n > 4, with
 *            a(1)=2, a(2)=15, a(3)=87, a(4)=500
 * Range:     u8 n <= 3, u16 n <= 6, u32 n <= 13, u64 n <= 25, u128 n <= 51,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0;
    if (n == 1) return (seq_t)2;
    if (n == 2) return (seq_t)15;
    if (n == 3) return (seq_t)87;
    if (n == 4) return (seq_t)500;

    seq_t a1 = (seq_t)2;     /* a(n-4) */
    seq_t a2 = (seq_t)15;    /* a(n-3) */
    seq_t a3 = (seq_t)87;    /* a(n-2) */
    seq_t a4 = (seq_t)500;   /* a(n-1) */
    seq_t next;

    for (idx_t i = 5; i <= n; ++i) {
        next = (seq_t)4 * a4 + (seq_t)12 * a3 - (seq_t)14 * a2 - a1;
        a1 = a2;
        a2 = a3;
        a3 = a4;
        a4 = next;
    }

    return a4;
}
