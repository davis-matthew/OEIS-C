/*
 * A276915  Indices of triangular numbers in A276914 which are also
 *          pentagonal.
 * https://oeis.org/A276915
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 14*a(n-1) - a(n-2) - 4*(-1)^n for n>1, a(0)=0, a(1)=1
 * Range:     u8 n <= 3, u16 n <= 5, u32 n <= 9, u64 n <= 17, u128 n <= 34,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* Handle the initial values directly. */
    if (n == 0) return 0;
    if (n == 1) return 1;

    /* Use an iterative O(n) computation.  We only keep the two previous
       terms, which is enough for the recurrence. */
    seq_t a_nm2 = 0;  /* a(n-2) */
    seq_t a_nm1 = 1;  /* a(n-1) */
    seq_t a_n   = 0;

    for (idx_t i = 2; i <= n; ++i)
    {
        if (i & 1)   /* i is odd: (-1)^i = -1, so add 4 */
            a_n = 14 * a_nm1 - a_nm2 + 4;
        else          /* i is even: (-1)^i =  1, so subtract 4 */
            a_n = 14 * a_nm1 - a_nm2 - 4;

        /* Shift the window for the next iteration. */
        a_nm2 = a_nm1;
        a_nm1 = a_n;
    }
    return a_n;
}
