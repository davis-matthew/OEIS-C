/*
 * A389176  Number of mutual-visibility sets in the n-antiprism graph.
 * https://oeis.org/A389176
 *
 * Offset:    3    (a(3) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = (7*n^6 + 210*n^5 + 1660*n^4 + 3960*n^3 + 11968*n^2 -
 *            24960*n + 5760) / 5760 for even n a(n) = (7*n^6 + 165*n^5 +
 *            1450*n^4 + 7470*n^3 - 18737*n^2 + 55725*n + 5760) / 5760 for odd
 *            n ≥ 5 a(3) = 63
 * Range:     u8 n <= 4, u16 n <= 15, u32 n <= 119, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n < 3)
        return (seq_t)0;   /* undefined for n < 3 */

    if (n == 3)
        return (seq_t)63;

    /* Compute with signed 128‑bit integer to avoid overflow for all
       n up to the limits of the tests (≈10 000). */
#if defined(__SIZEOF_INT128__)
    __int128 N = n;      /* use signed 128-bit type */
    __int128 n2 = N * N;
    __int128 n3 = n2 * N;
    __int128 n4 = n3 * N;
    __int128 n5 = n4 * N;
    __int128 n6 = n5 * N;

    __int128 num;
    if ((n & 1) == 0)   /* even n */
    {
        num = 7 * n6
            + (__int128)210 * n5
            + (__int128)1660 * n4
            + (__int128)3960 * n3
            + (__int128)11968 * n2
            - (__int128)24960 * N
            + 5760;
    }
    else                /* odd n (>=5) */
    {
        num = 7 * n6
            + (__int128)165 * n5
            + (__int128)1450 * n4
            + (__int128)7470 * n3
            - (__int128)18737 * n2
            + (__int128)55725 * N
            + 5760;
    }

    __int128 res = num / 5760;
    return (seq_t)res;
#else
    /* Fallback: compute using 128‑bit arithmetic via unsigned long long
       when __int128 is not available. This branch is not expected to be
       hit in the supported build environments for this sequence. */
    abort();
#endif
}
