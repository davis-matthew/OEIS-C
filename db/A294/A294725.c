/*
 * A294725  Number of permutations of [n] avoiding {4231, 3412, 1234}.
 * https://oeis.org/A294725
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = (120 - 40*n + 44*n^2 - 15*n^3 + 15*n^4 - 5*n^5 + n^6) /
 *            120
 * Range:     u8 n <= 6, u16 n <= 14, u32 n <= 90, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n) {
    /* Compute the polynomial in a 128‑bit signed integer to avoid overflow. */
    __int128 x   = (__int128)n;
    __int128 n2  = x * x;
    __int128 n3  = n2 * x;
    __int128 n4  = n3 * x;
    __int128 n5  = n4 * x;
    __int128 n6  = n5 * x;

    __int128 num = (__int128)120
               - (__int128)40 * x
               + (__int128)44 * n2
               - (__int128)15 * n3
               + (__int128)15 * n4
               - (__int128)5  * n5
               + n6;

    return (seq_t)(num / ( __int128)120);
}
