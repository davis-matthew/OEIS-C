/*
 * A367118  Place n points in general position on each side of an equilateral
 *          triangle, and join every pair of the 3*n+3 boundary points by a
 *          chord; sequence gives number of regions in the resulting planar
 *          graph.
 * https://oeis.org/A367118
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = (1/4)*(9*n^4 + 12*n^3 + 15*n^2 + 12*n + 4)
 * Range:     u8 n <= 2, u16 n <= 12, u32 n <= 37, u64 n <= 37, u128 n <= 37,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* compute with a precision large enough for all needed n */
#if defined(__SIZEOF_INT128__) && !defined(_MSC_VER)
    unsigned __int128 t = (unsigned __int128)n;   /* n    */
    t = t * 9 + 12;                              /* 9n + 12 */
    t = t * n + 15;                              /* (9n+12)n + 15 */
    t = t * n + 12;                              /* ((9n+12)n + 15)n + 12 */
    t = t * n + 4;                               /* ((((9n+12)n + 15)n + 12)n) + 4 */
    t /= 4;                                      /* divide by 4 */
    return (seq_t)t;
#else
    unsigned long long t = (unsigned long long)n;
    t = t * 9ULL + 12ULL;
    t = t * n + 15ULL;
    t = t * n + 12ULL;
    t = t * n + 4ULL;
    t /= 4ULL;
    return (seq_t)t;
#endif
}
