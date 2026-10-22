/*
 * A377714  Cogrowth sequence for the 8-element group C4 X C2 = <S,T | S^4,
 *          T^2, [S,T]>.
 * https://oeis.org/A377714
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(0) = 1 a(1) = 1 a(2) = 2 a(3) = 16 a(n) = 4*a(n-1) - 4*a(n-2)
 *            + 16*a(n-3) for n > 3 (OEIS A377714 – cogrowth of C4 × C2)
 * Range:     u8 n <= 4, u16 n <= 8, u32 n <= 16, u64 n <= 25, u128 n <= 25,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* Handle the four base cases */
    if (n == 0) return (seq_t)1;
    if (n == 1) return (seq_t)1;
    if (n == 2) return (seq_t)2;
    if (n == 3) return (seq_t)16;

    /* iterative computation using the recurrence */
    seq_t a_m3 = (seq_t)1;  /* a(n-3)   -> a(1)  */
    seq_t a_m2 = (seq_t)2;  /* a(n-2)   -> a(2)  */
    seq_t a_m1 = (seq_t)16; /* a(n-1)   -> a(3)  */
    seq_t cur  = a_m1;      /* placeholder for a(n)    */

    for (idx_t i = 4; i <= n; ++i)
    {
        cur = (seq_t)4 * a_m1 - (seq_t)4 * a_m2 + (seq_t)16 * a_m3;
        /* Shift the window for the next iteration */
        a_m3 = a_m2;
        a_m2 = a_m1;
        a_m1 = cur;
    }

    return cur;
}
