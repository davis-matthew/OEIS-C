/*
 * A275060  Numbers n such that there exists x in N : (x+1)^3 - x^3 = 61*n^2.
 * https://oeis.org/A275060
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 974*a(n-1) - a(n-2) for n>2; a(1)=1, a(2)=973
 * Range:     u8 n <= 1, u16 n <= 2, u32 n <= 4, u64 n <= 7, u128 n <= 13,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 0)
        return 0;                  /* outside the offset, undefined */
    if (n == 1)
        return (seq_t)1;
    if (n == 2)
        return (seq_t)973;

    seq_t a0 = (seq_t)1;            /* a(1) */
    seq_t a1 = (seq_t)973;          /* a(2) */
    for (idx_t i = 3; i <= n; ++i)
    {
        seq_t a2 = (seq_t)974 * a1 - a0;  /* recurrence */
        a0 = a1;
        a1 = a2;
    }
    return a1;
}
