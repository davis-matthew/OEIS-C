/*
 * A319577  a(n) = (4/45)*n*(n - 2)*(n - 1)*(n^3 - 12*n^2 + 47*n - 15).
 * https://oeis.org/A319577
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = (4/45)*n*(n-1)*(n-2)*(n^3-12*n^2+47*n-15)
 * Range:     u8 n <= 5, u16 n <= 12, u32 n <= 62, u64 n <= 2435,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* All terms vanish for n < 3 because of the factor (n-2). */
    if (n < 3)
        return 0;

    /* Perform all arithmetic in a wider type to avoid intermediate overflow
       (uint128 is available on GCC/Clang for C11/C++17). */
    __int128 nn   = (__int128)n;
    __int128 n1   = nn - 1;     /* (n-1) */
    __int128 n2   = nn - 2;     /* (n-2) */
    __int128 poly = nn*nn*nn - 12*nn*nn + 47*nn - 15; /* n^3 - 12n^2 + 47n - 15 */

    /* Compute the product and divide by 45. */
    __int128 prod = nn * n1 * n2 * poly * 4;
    prod /= 45;

    return (seq_t)prod;
}
