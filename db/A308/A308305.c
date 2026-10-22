/*
 * A308305  a(n) = s(n,n) + s(n,n-1) + s(n,n-2), where s(n,k) are the unsigned
 *          Stirling numbers of the first kind (see A132393).
 * https://oeis.org/A308305
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = (1/24)*(3*n^4 - 10*n^3 + 21*n^2 - 14*n + 24)
 * Range:     u8 n <= 3, u16 n <= 12, u32 n <= 260, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t N   = (seq_t)n;
    seq_t n2  = N * N;          // n^2
    seq_t n3  = n2 * N;         // n^3
    seq_t n4  = n3 * N;         // n^4

    seq_t num = 3 * n4;         // 3*n^4
    num -= 10 * n3;             // -10*n^3
    num += 21 * n2;             // +21*n^2
    num -= 14 * N;              // -14*n
    num += 24;                  // +24

    return num / 24;            // all integers, division exact
}
