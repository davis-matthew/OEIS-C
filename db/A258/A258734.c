/*
 * A258734  Number of length n+4 0..3 arrays with at most one downstep in
 *          every n consecutive neighbor pairs.
 * https://oeis.org/A258734
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = (n^7 + 56 n^6 + 1330 n^5 + 16520 n^4 + 332689 n^3 * +
 *            1626464 n^2 + 2397660 n + 504000) / 5040 for n > 2 * and a(1) =
 *            1024, a(2) = 3136. The formula was empirically * observed to
 *            hold for all n ≥ 3
 * Range:     u8 none, u16 n <= 7, u32 n <= 72, u64 n <= 210, u128 n <= 210,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 1) return 1024;
    if (n == 2) return 3136;

    __int128 N = n;          /* use 128‑bit arithmetic for intermediate values */

    __int128 p2 = N * N;        /* n^2  */
    __int128 p3 = p2 * N;       /* n^3  */
    __int128 p4 = p3 * N;       /* n^4  */
    __int128 p5 = p4 * N;       /* n^5  */
    __int128 p6 = p5 * N;       /* n^6  */
    __int128 p7 = p6 * N;       /* n^7  */

    /* Compute the numerator of the rational expression */
    __int128 num = p7
        + (__int128)56   * p6
        + (__int128)1330 * p5
        + (__int128)16520* p4
        + (__int128)332689* p3
        + (__int128)1626464* p2
        + (__int128)2397660* N
        + (__int128)504000;

    /* Division yields an integer for all n ≥ 3 */
    return (seq_t)(num / 5040);
}
