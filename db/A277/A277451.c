/*
 * A277451  Number of edges in geodesic dome generated from icosahedron by
 *          recursively dividing each triangle in 4.
 * https://oeis.org/A277451
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(0) = 1; a(n) = 30 * 4^(n-1) for n > 0
 * Range:     u8 n <= 2, u16 n <= 6, u32 n <= 14, u64 n <= 30, u128 n <= 62,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 0)
        return (seq_t)1;          /* a(0) */

    /* Compute 4^(n-1) using exponentiation by squaring */
    seq_t pow4   = (seq_t)1;
    seq_t base   = (seq_t)4;
    idx_t exp    = n - 1;         /* n >= 1, so exp is valid */

    while (exp != 0) {
        if (exp & 1)
            pow4 *= base;
        base *= base;
        exp >>= 1;
    }

    /* Multiply by 30 to get the required term */
    return (seq_t)30 * pow4;
}
