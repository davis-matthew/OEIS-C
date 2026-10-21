/*
 * A297047  Number of edge covers in the n-wheel graph.
 * https://oeis.org/A297047
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 4*a(n-1) - 5*a(n-3) - 2*a(n-4); Initial values: a(1)=0,
 *            a(2)=2, a(3)=10, a(4)=41
 * Range:     u8 n <= 5, u16 n <= 9, u32 n <= 18, u64 n <= 35, u128 n <= 70,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 1) return (seq_t)0;
    if (n == 2) return (seq_t)2;
    if (n == 3) return (seq_t)10;
    if (n == 4) return (seq_t)41;

    seq_t a1 = (seq_t)0;   /* a(n-4) */
    seq_t a2 = (seq_t)2;   /* a(n-3) */
    seq_t a3 = (seq_t)10;  /* a(n-2) */
    seq_t a4 = (seq_t)41;  /* a(n-1) */

    for (idx_t i = 5; i <= n; ++i) {
        seq_t next = (seq_t)4 * a4 - (seq_t)5 * a2 - (seq_t)2 * a1;
        a1 = a2;
        a2 = a3;
        a3 = a4;
        a4 = next;
    }
    return a4;
}
