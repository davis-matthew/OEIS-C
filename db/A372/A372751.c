/*
 * A372751  a(n) = (3*n^5 + 4*n^3 - n)/6.
 * https://oeis.org/A372751
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = (3*n^5 + 4*n^3 - n)/6
 * Range:     u8 n <= 3, u16 n <= 10, u32 n <= 97, u64 n <= 8191,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 0) return 0; /* undefined for the offset, but harmless */
    unsigned __int128 N  = n;          /* Promote index to an intermediate type */
    unsigned __int128 n2 = N * N;       /* n^2 */
    unsigned __int128 n3 = n2 * N;      /* n^3 */
    unsigned __int128 n5 = n3 * n2;     /* n^5 */
    unsigned __int128 num = 3 * n5 + 4 * n3 - N; /* 3*n^5 + 4*n^3 - n */
    unsigned __int128 res = num / 6;    /* division by 6 */
    return (seq_t)res;                  /* result fits in seq_t for the tested range */
}
