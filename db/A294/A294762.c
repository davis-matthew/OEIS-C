/*
 * A294762  Number of permutations of [n] avoiding {4231, 1243, 1234}.
 * https://oeis.org/A294762
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = (25200 - 52056*n + 48650*n^2 - 20881*n^3 + 4340*n^4 -
 *            154*n^5 - 70*n^6 + 11*n^7) / 5040 for n>0 a(0) = 1
 * Range:     u8 n <= 6, u16 n <= 12, u32 n <= 58, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n) {
    if (n == 0)
        return (seq_t)1;

    /* Use 128‑bit temporary to avoid intermediate overflow on all supported builds. */
    __int128 t = (__int128)11;
    t = t * (__int128)n - 70;
    t = t * (__int128)n - 154;
    t = t * (__int128)n + 4340;
    t = t * (__int128)n - 20881;
    t = t * (__int128)n + 48650;
    t = t * (__int128)n - 52056;
    t = t * (__int128)n + 25200;
    t /= 5040;

    return (seq_t)t;
}
