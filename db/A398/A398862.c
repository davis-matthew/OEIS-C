/*
 * A398862  Number of points with even coordinate sum in the cube [-n,n]^3
 *          having an odd number of lattice neighbors inside the cube.
 * https://oeis.org/A398862
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 12*n*(n-1) + 7*(1 + (-1)^n)
 * Range:     u8 n <= 5, u16 n <= 74, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*           a(n) = 12*n*(n-1) + 14      if n is even */
/*           a(n) = 12*n*(n-1)            if n is odd  */
seq_t a(idx_t n)
{
    /* Avoid any operations below the offset */
    if (n == 0) return (seq_t)0;

    seq_t sn = (seq_t)n;
    seq_t res = sn * (sn - (seq_t)1);   /* sn*(n-1) */
    res *= (seq_t)12;                  /* *12 */

    /* Add 14 when n is even; +0 otherwise */
    if ((n & (idx_t)1) == 0)
        res += (seq_t)14;

    return res;
}
