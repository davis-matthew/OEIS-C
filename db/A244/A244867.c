/*
 * A244867  Let G denote the 9-node, 16-edge graph formed from an octagon with
 *          main diagonals drawn and a node at the center; a(n) = number of
 *          magic labelings of G with magic sum 2n.
 * https://oeis.org/A244867
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = (5040 + 22164*n + 43092*n^2 + 46963*n^3 + 30240*n^4 +
 *            11326*n^5 + 2268*n^6 + 187*n^7) / 5040 (Polynomial evaluation
 *            with O(1) time, safe from overflow for building types of 64 or
 *            128 bits.)
 * Range:     u8 n <= 1, u16 n <= 6, u32 n <= 36, u64 n <= 903, u128 n <= 1000,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* Coefficients of the numerator polynomial, as 64‑bit constants. */
    static const uint64_t coeff[8] = {
        5040ULL, 22164ULL, 43092ULL, 46963ULL,
        30240ULL, 11326ULL, 2268ULL,  187ULL
    };

    /* Use a wider type for intermediate arithmetic.  Unsigned
     * __int128 is guaranteed to be at least 128 bits on the target
     * systems for which the build is expected.  If seq_t is already
     * 128 bits, the cast in the return statement is loss‑free. */
    __int128 acc  = 0;
    __int128 pow  = 1;
    uint64_t nn   = (uint64_t)n;   /* promote one time */

    for (int i = 0; i < 8; ++i) {
        acc += (__int128)coeff[i] * pow;
        pow *= nn;
    }

    return (seq_t)(acc / 5040);
}
