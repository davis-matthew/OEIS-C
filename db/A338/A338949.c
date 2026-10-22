/*
 * A338949  Number of unoriented colorings of the 24 octahedral facets (or 24
 *          vertices) of the 4-D 24-cell using subsets of a set of n colors.
 * https://oeis.org/A338949
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = (96*n^2 + 144*n^3 + 144*n^4 + 140*n^6 + 300*n^7 + 120*n^8
 *            + 36*n^9 + 45*n^12 + 84*n^13 + 18*n^14 + 12*n^15 + 12*n^18 +
 *            n^24) / 1152
 * Range:     u8 n <= 1, u16 n <= 2, u32 n <= 3, u64 n <= 8, u128 n <= 30,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
#if defined(__cplusplus)
    /* C++ build: seq_t is an arbitrary‑precision type; use direct arithmetic */
    seq_t sn = (seq_t)n;
    seq_t n2 = sn * sn;
    seq_t n3 = n2 * sn;
    seq_t n4 = n3 * sn;
    seq_t n6 = n4 * n2;
    seq_t n7 = n6 * sn;
    seq_t n8 = n4 * n4;
    seq_t n9 = n8 * sn;
    seq_t n12 = n6 * n6;
    seq_t n13 = n12 * sn;
    seq_t n14 = n12 * n2;
    seq_t n15 = n12 * n3;
    seq_t n18 = n6 * n12;
    seq_t n24 = n12 * n12;

    seq_t num = (seq_t)96  * n2
              + (seq_t)144 * n3
              + (seq_t)144 * n4
              + (seq_t)140 * n6
              + (seq_t)300 * n7
              + (seq_t)120 * n8
              + (seq_t) 36 * n9
              + (seq_t) 45 * n12
              + (seq_t) 84 * n13
              + (seq_t) 18 * n14
              + (seq_t) 12 * n15
              + (seq_t) 12 * n18
              + (seq_t)  1 * n24;

    return num / (seq_t)1152;
#else
    /* C build: use 128‑bit intermediates to avoid overflow */
    __int128 sn = (__int128)(idx_t)n;
    __int128 n2 = sn * sn;          /* n^2  */
    __int128 n3 = n2 * sn;          /* n^3  */
    __int128 n4 = n3 * sn;          /* n^4  */
    __int128 n6 = n4 * n2;          /* n^6  */
    __int128 n7 = n6 * sn;          /* n^7  */
    __int128 n8 = n4 * n4;          /* n^8  */
    __int128 n9 = n8 * sn;          /* n^9  */
    __int128 n12 = n6 * n6;         /* n^12 */
    __int128 n13 = n12 * sn;        /* n^13 */
    __int128 n14 = n12 * n2;        /* n^14 */
    __int128 n15 = n12 * n3;        /* n^15 */
    __int128 n18 = n6 * n12;        /* n^18 */
    __int128 n24 = n12 * n12;       /* n^24 */

    __int128 num = (__int128)96  * n2
                 + (__int128)144 * n3
                 + (__int128)144 * n4
                 + (__int128)140 * n6
                 + (__int128)300 * n7
                 + (__int128)120 * n8
                 + (__int128) 36 * n9
                 + (__int128) 45 * n12
                 + (__int128) 84 * n13
                 + (__int128) 18 * n14
                 + (__int128) 12 * n15
                 + (__int128) 12 * n18
                 + (__int128)  1 * n24;

    return (seq_t)(num / (__int128)1152);
#endif
}
