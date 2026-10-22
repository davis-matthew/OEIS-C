/*
 * A387624  a(n) = Sum_{k=0..floor(n/2)} 2^k * binomial(2*n-2*k+1,2*k).
 * https://oeis.org/A387624
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 2*a(n-1) + 3*a(n-2) + 4*a(n-3) - 4*a(n-4) Initial values:
 *            a(0)=1, a(1)=1, a(2)=7, a(3)=21
 * Range:     u8 n <= 5, u16 n <= 9, u32 n <= 19, u64 n <= 38, u128 n <= 76,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 0)
        return 1;
    if (n == 1)
        return 1;
    if (n == 2)
        return 7;
    if (n == 3)
        return 21;

    seq_t a0 = 1;  /* a(n-4)  */
    seq_t a1 = 1;  /* a(n-3)  */
    seq_t a2 = 7;  /* a(n-2)  */
    seq_t a3 = 21; /* a(n-1)  */

    for (idx_t i = 4; i <= n; ++i) {
        seq_t ai = 2 * a3 + 3 * a2 + 4 * a1 - 4 * a0;
        a0 = a1;
        a1 = a2;
        a2 = a3;
        a3 = ai;
    }
    return a3;
}
