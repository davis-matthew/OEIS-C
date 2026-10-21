/*
 * A247355  Number of paths from (0,1) to (n,2), with vertices (i,k)
 *          satisfying 0 <= k <= 3, consisting of segments given by the
 *          vectors (1,1), (1,2), (1,-1).
 * https://oeis.org/A247355
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 3*a(n-2) + 2*a(n-3) - a(n-4) with a(0)=0, a(1)=1, a(2)=2,
 *            a(3)=3 (OEIS A247355)
 * Range:     u8 n <= 9, u16 n <= 17, u32 n <= 34, u64 n <= 67, u128 n <= 134,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* handle the first four terms directly */
    if (n == 0) return (seq_t)0;
    if (n == 1) return (seq_t)1;
    if (n == 2) return (seq_t)2;
    if (n == 3) return (seq_t)3;

    /* values for indices (n-4), (n-3), (n-2), (n-1) respectively */
    seq_t v0 = (seq_t)0;   /* a(0) */
    seq_t v1 = (seq_t)1;   /* a(1) */
    seq_t v2 = (seq_t)2;   /* a(2) */
    seq_t v3 = (seq_t)3;   /* a(3) */
    seq_t vi;

    for (idx_t i = 4; i <= n; ++i)
    {
        /* vi = 3*v2 + 2*v1 - v0, implemented with + and - only */
        vi = v2 + v2;   /* 2*v2 */
        vi = vi + v2;   /* 3*v2 */
        vi = vi + v1;   /* +v1 */
        vi = vi + v1;   /* +v1 */
        vi = vi - v0;   /* -v0 */

        /* shift the window: v0<-v1, v1<-v2, v2<-v3, v3<-vi */
        v0 = v1;
        v1 = v2;
        v2 = v3;
        v3 = vi;
    }
    return v3;
}
