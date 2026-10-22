/*
 * A330987  Alternatively add and half-multiply pairs of the nonnegative
 *          integers.
 * https://oeis.org/A330987
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 4n-3 for odd n, 2n^2-3n+1 for even n. (Defined for n ≥ 1,
 *            offset = 1.)
 * Range:     u8 n <= 13, u16 n <= 181, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t N = (seq_t)n;
    if (n & 1) {                   /* odd index */
        return (N << 2) - 3;       /* 4*n - 3 */
    } else {                       /* even index */
        seq_t Nsqr = N * N;        /* n^2 */
        return (Nsqr << 1) - (N * 3) + 1; /* 2*n^2 - 3*n + 1 */
    }
}
