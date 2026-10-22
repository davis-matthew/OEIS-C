/*
 * A354608  Number of quadruples (p_1, ..., p_4) of positive integers such
 *          that p_{i-1} <= p_i <= n^(i-1).
 * https://oeis.org/A354608
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = (6*n^6-6*n^5+3*n^4+4*n^3-3*n^2+2*n)/6
 * Range:     u8 n <= 2, u16 n <= 6, u32 n <= 40, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    unsigned __int128 nn = n;
    unsigned __int128 n1 = nn;
    unsigned __int128 n2 = n1 * n1;
    unsigned __int128 n3 = n2 * n1;
    unsigned __int128 n4 = n3 * n1;
    unsigned __int128 n5 = n4 * n1;
    unsigned __int128 n6 = n5 * n1;

    unsigned __int128 temp = 6 * n6 - 6 * n5 + 3 * n4 + 4 * n3 - 3 * n2 + 2 * n1;
    return (seq_t)(temp / 6);
}
