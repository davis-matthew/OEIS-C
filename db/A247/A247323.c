/*
 * A247323  Number of paths from (0,0) to (n,0), with vertices (i,k)
 *          satisfying 0 <= k <= 3, consisting of segments given by the
 *          vectors (1,1), (1,2), (1,-1).
 * https://oeis.org/A247323
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 11, u16 n <= 19, u32 n <= 36, u64 n <= 69, u128 n <= 136,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/**********************************************************************
 * Sequence A247323
 *
 * Recurrence: a(n) = 3*a(n-2) + 2*a(n-3) - a(n-4)
 * Initial values:
 *   a(0) = 1, a(1) = 0, a(2) = 1, a(3) = 1
 *
 * The function computes the term for any n >= 0 using only O(1)
 * auxiliary storage.
 **********************************************************************/
seq_t a(idx_t n)
{
    /* Handle the first four base cases directly. */
    if (n == (idx_t)0) return (seq_t)1;
    if (n == (idx_t)1) return (seq_t)0;
    if (n == (idx_t)2) return (seq_t)1;
    if (n == (idx_t)3) return (seq_t)1;

    /* A small circular buffer that always keeps
       a[i-4], a[i-3], a[i-2] and a[i-1].  We use
       indices modulo 4 to avoid using extra storage.   */
    seq_t buf[4] = { (seq_t)1, (seq_t)0, (seq_t)1, (seq_t)1 };
    /* buf[0] = a(0), buf[1] = a(1), buf[2] = a(2), buf[3] = a(3) */

    for (idx_t i = (idx_t)4; i <= n; ++i)
    {
        /* For each i, indexes are taken modulo 4. */
        seq_t cur = (seq_t)3 * buf[(i - (idx_t)2) & (idx_t)3]
                      + (seq_t)2 * buf[(i - (idx_t)3) & (idx_t)3]
                      - buf[(i - (idx_t)4) & (idx_t)3];
        buf[i & (idx_t)3] = cur;
    }

    return buf[n & (idx_t)3];
}
