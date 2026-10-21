/*
 * A247326  Number of paths from (0,0) to (n,3), with vertices (i,k)
 *          satisfying 0 <= k <= 3, consisting of segments given by the
 *          vectors (1,1), (1,2), (1,-1).
 * https://oeis.org/A247326
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 3*a(n-2) + 2*a(n-3) - a(n-4) for n ≥ 4 with initial
 *            values a(0)=0, a(1)=0, a(2)=2, a(3)=2
 * Range:     u8 n <= 9, u16 n <= 18, u32 n <= 34, u64 n <= 68, u128 n <= 135,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* Handle the four base cases directly */
    if (n == 0 || n == 1) return 0;
    if (n == 2 || n == 3) return 2;

    /* Maintain the last four values of the sequence.
       a0 = a(i-4), a1 = a(i-3), a2 = a(i-2), a3 = a(i-1)  */
    seq_t a0 = 0, a1 = 0, a2 = 2, a3 = 2, a4;

    for (idx_t i = 4; i <= n; ++i) {
        a4 = (seq_t)3 * a2 + (seq_t)2 * a1 - a0;    /* recurrence */
        a0 = a1;          /* shift window for next iteration */
        a1 = a2;
        a2 = a3;
        a3 = a4;
    }
    return a3;   /* a3 now holds a(n) */
}
