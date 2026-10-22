/*
 * A394275  Number of induced cubes in the n-Sierpinski tetrahedron graph.
 * https://oeis.org/A394275
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 35*4^(n-2) + 2 (n > 1); a(1) = 10
 * Range:     u8 n <= 3, u16 n <= 7, u32 n <= 15, u64 n <= 24, u128 n <= 24,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 1)
        return (seq_t)10;

    idx_t exp = n - 2;          /* exponent of 4 */
    seq_t result = (seq_t)1;    /* 4^0 = 1 */
    seq_t base = (seq_t)4;      /* base 4 */

    /* binary exponentiation: compute 4^(n-2) */
    while (exp)
    {
        if (exp & (idx_t)1)
            result *= base;
        base *= base;
        exp >>= 1;
    }

    /* a(n) = 35 * 4^(n-2) + 2 */
    return (seq_t)35 * result + (seq_t)2;
}
