/*
 * A392420  Number of mutual-visibility sets in the n X 4 grid graph.
 * https://oeis.org/A392420
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = (8*n^8 + 32*n^7 - 107*n^6 + 179*n^5 + 2767*n^4 + 2573*n^3
 *            - 31108*n^2 + 46176*n + 3960) / 360 for n >= 3 a(1) = 11, a(2) =
 *            94
 * Range:     u8 n <= 2, u16 n <= 6, u32 n <= 25, u64 n <= 411, u128 n <= 1000,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == (idx_t)1) return (seq_t)11;
    if (n == (idx_t)2) return (seq_t)94;
    if (n < (idx_t)3)  return (seq_t)0;   /* undefined below offset */

    unsigned __int128 N   = (unsigned __int128)n;
    unsigned __int128 n2  = N * N;
    unsigned __int128 n3  = n2 * N;
    unsigned __int128 n4  = n3 * N;
    unsigned __int128 n5  = n4 * N;
    unsigned __int128 n6  = n5 * N;
    unsigned __int128 n7  = n6 * N;
    unsigned __int128 n8  = n7 * N;

    /* Sum of positive contributions */
    unsigned __int128 pos = (unsigned __int128)8  * n8
                          + (unsigned __int128)32 * n7
                          + (unsigned __int128)179 * n5
                          + (unsigned __int128)2767 * n4
                          + (unsigned __int128)2573 * n3
                          + (unsigned __int128)46176 * N
                          + (unsigned __int128)3960;

    /* Sum of negative contributions */
    unsigned __int128 neg = (unsigned __int128)107 * n6
                          + (unsigned __int128)31108 * n2;

    unsigned __int128 num = pos - neg;
    num /= (unsigned __int128)360;

    return (seq_t)num;
}
