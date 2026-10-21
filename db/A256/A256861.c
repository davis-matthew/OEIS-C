/*
 * A256861  a(n) = n*(n + 1)*(n + 2)*(n + 3)*(n + 4)*(n^2 - n + 6)/720.
 * https://oeis.org/A256861
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = n*(n + 1)*(n + 2)*(n + 3)*(n + 4)*(n^2 - n + 6)/720
 * Range:     u8 n <= 4, u16 n <= 11, u32 n <= 32, u64 n <= 32, u128 n <= 32,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* Use a 128‑bit intermediate to perform the product safely before the division. */
    __int128 prod = (__int128)n;
    prod *= (n + 1);
    prod *= (n + 2);
    prod *= (n + 3);
    prod *= (n + 4);
    prod *= ( (__int128)n * n - n + 6 );
    prod /= 720;
    return (seq_t)prod;
}
