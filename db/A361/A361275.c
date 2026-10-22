/*
 * A361275  Number of 1423-avoiding even Grassmannian permutations of size n.
 * https://oeis.org/A361275
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = (24 - 5*n + 2*n^3 - 3*(-1)^n * n) / 24
 * Range:     u8 n <= 4, u16 n <= 32, u32 n <= 2869, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t N   = (seq_t)n;
    seq_t n2  = N * N;          /* n^2 */
    seq_t n3  = n2 * N;         /* n^3 */

    seq_t num = (seq_t)2 * n3 + (seq_t)24
                - (seq_t)5 * N;          /* 2*n^3 + 24 - 5*n */

    if ((n & 1ULL) == 0) {            /* even n: subtract 3*n */
        num -= (seq_t)3 * N;
    } else {                          /* odd n: add 3*n */
        num += (seq_t)3 * N;
    }

    return num / (seq_t)24;           /* divide by 24 */
}
