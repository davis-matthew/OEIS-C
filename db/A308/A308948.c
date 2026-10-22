/*
 * A308948  a(n) = A006190(A322907(n)+1) mod n.
 * https://oeis.org/A308948
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = A006190(A322907(n)+1) mod n Computed by repeatedly
 *            multiplying the matrix M = [[3,1],[1,0]] modulo n until it
 *            becomes a scalar matrix
 * Range:     u8 n <= 67, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* a(1) is defined to be 0. */
    if (n == 1) return 0;

    seq_t mod = (seq_t)n;               /* modulo base */

    /* Initial matrix M = [[3,1],[1,0]] (mod n). */
    seq_t m00 = 3 % mod;
    seq_t m01 = 1 % mod;
    seq_t m10 = 1 % mod;
    seq_t m11 = 0 % mod;

    /* Current power of M, starting with M¹. */
    seq_t a00 = m00, a01 = m01, a10 = m10, a11 = m11;

    /* Iterate until the off‑diagonal elements are zero
       (the matrix becomes r·I mod n). */
    while (!(a01 == 0 && a10 == 0))
    {
        /* Multiply current matrix by M, modulo n. */
        seq_t t00 = (unsigned __int128)a00 * m00 + (unsigned __int128)a01 * m10;
        seq_t t01 = (unsigned __int128)a00 * m01 + (unsigned __int128)a01 * m11;
        seq_t t10 = (unsigned __int128)a10 * m00 + (unsigned __int128)a11 * m10;
        seq_t t11 = (unsigned __int128)a10 * m01 + (unsigned __int128)a11 * m11;

        a00 = t00 % mod;
        a01 = t01 % mod;
        a10 = t10 % mod;
        a11 = t11 % mod;
    }

    /* The scalar r is stored in both diagonal entries. */
    return a00 % mod;
}
