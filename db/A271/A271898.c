/*
 * A271898  Expansion of ( 1+x^2 ) / ( 1-4*x+5*x^2-4*x^3 ).
 * https://oeis.org/A271898
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 4*a(n-1) - 5*a(n-2) + 4*a(n-3), with initial values
 *            a(0)=1, a(1)=4, a(2)=12
 * Range:     u8 n <= 5, u16 n <= 10, u32 n <= 21, u64 n <= 44, u128 n <= 89,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)1;
    if (n == 1) return (seq_t)4;
    if (n == 2) return (seq_t)12;

    seq_t a0 = (seq_t)1;  /* a(n-3)   */
    seq_t a1 = (seq_t)4;  /* a(n-2)   */
    seq_t a2 = (seq_t)12; /* a(n-1)   */

    for (idx_t i = 3; i <= n; ++i) {
        seq_t next = 4 * a2 + 4 * a0 - 5 * a1;
        a0 = a1;
        a1 = a2;
        a2 = next;
    }
    return a2;
}
