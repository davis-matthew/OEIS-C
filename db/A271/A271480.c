/*
 * A271480  Dimension of n-qubit subspace H^{MPS}_{2,n} for bond dimension 2.
 * https://oeis.org/A271480
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, more
 * Formula:   a(n) = (n^4 + 4*n^3 + 2*n^2 + 44*n + (48 if n even else 45)) /
 *            48
 * Range:     u8 n <= 9, u16 n <= 15, u32 n <= 15, u64 n <= 15, u128 n <= 15,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t N = (seq_t)n;
    if ((n & 1U) == 0)    /* n even */
    {
        return (N*N*N*N + 4ULL*N*N*N + 2ULL*N*N + 44ULL*N + 48ULL) / 48ULL;
    }
    else                 /* n odd */
    {
        return (N*N*N*N + 4ULL*N*N*N + 2ULL*N*N + 44ULL*N + 45ULL) / 48ULL;
    }
}
