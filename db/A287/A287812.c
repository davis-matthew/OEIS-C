/*
 * A287812  Number of octonary sequences of length n such that no two
 *          consecutive terms have distance 1.
 * https://oeis.org/A287812
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 7*a(n-1) - 3*a(n-2) - 10*a(n-3) + 3*a(n-4) with initial
 *            values a(0)=1, a(1)=8, a(2)=50, a(3)=314, a(4)=1972
 * Range:     u8 n <= 2, u16 n <= 5, u32 n <= 11, u64 n <= 21, u128 n <= 21,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* Handle the base cases directly. */
    if (n == 0) return (seq_t)1;
    if (n == 1) return (seq_t)8;
    if (n == 2) return (seq_t)50;
    if (n == 3) return (seq_t)314;
    if (n == 4) return (seq_t)1972;

    /* Use a sliding window of the last four values. */
    seq_t a0 = (seq_t)1;    /* a(n-4) */
    seq_t a1 = (seq_t)8;    /* a(n-3) */
    seq_t a2 = (seq_t)50;   /* a(n-2) */
    seq_t a3 = (seq_t)314;  /* a(n-1) */
    seq_t a4 = (seq_t)1972; /* a(n)   for starting point n=4 */

    /* Iterate to the desired index. */
    for (idx_t i = 5; i <= n; ++i)
    {
        seq_t next = 7 * a4 - 3 * a3 - 10 * a2 + 3 * a1; /* a(i) */
        a1 = a2;
        a2 = a3;
        a3 = a4;
        a4 = next;
    }
    return a4;
}
