/*
 * A359987  Number of edge cuts in the n-ladder graph P_2 X P_n.
 * https://oeis.org/A359987
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 13*a(n-1) - 42*a(n-2) + 16*a(n-3) for n > 3; initial
 *            values: a(1)=1, a(2)=11, a(3)=105
 * Range:     u8 n <= 3, u16 n <= 6, u32 n <= 11, u64 n <= 22, u128 n <= 43,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 0) return 0;                     /* offset 1 → a(1) is first term */
    if (n == 1) return 1;
    if (n == 2) return 11;
    if (n == 3) return 105;

    seq_t a1 = 1;   /* a(1)  */
    seq_t a2 = 11;  /* a(2)  */
    seq_t a3 = 105; /* a(3)  */

    for (idx_t i = 4; i <= n; ++i) {
        seq_t next = 13 * a3 - 42 * a2 + 16 * a1;
        a1 = a2;
        a2 = a3;
        a3 = next;
    }
    return a3;
}
