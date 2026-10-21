/*
 * A280392  Number of nX2 0..2 arrays with no element unequal to a strict
 *          majority of its horizontal and antidiagonal neighbors, with the
 *          exception of exactly one element, and with new values introduced
 *          in order 0 sequentially upwards.
 * https://oeis.org/A280392
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 10*a(n-1) -33*a(n-2) +40*a(n-3) -16*a(n-4) with initial
 *            values a(1)=0, a(2)=2, a(3)=14, a(4)=74
 * Range:     u8 n <= 4, u16 n <= 8, u32 n <= 16, u64 n <= 31, u128 n <= 63,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == (idx_t)1) return (seq_t)0;
    if (n == (idx_t)2) return (seq_t)2;
    if (n == (idx_t)3) return (seq_t)14;
    if (n == (idx_t)4) return (seq_t)74;

    seq_t a1 = (seq_t)0;  /* a(n-4)  */
    seq_t a2 = (seq_t)2;  /* a(n-3)  */
    seq_t a3 = (seq_t)14; /* a(n-2)  */
    seq_t a4 = (seq_t)74; /* a(n-1)  */

    for (idx_t i = (idx_t)5; i <= n; ++i) {
        seq_t t1 = a4 * (seq_t)10 + a2 * (seq_t)40;
        seq_t t2 = a3 * (seq_t)33 + a1 * (seq_t)16;
        seq_t next = t1 - t2;          /* t1 >= t2 for all n ≥ 5 */

        a1 = a2;
        a2 = a3;
        a3 = a4;
        a4 = next;
    }
    return a4;
}
