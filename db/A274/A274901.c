/*
 * A274901  Number of (not necessarily proper) face colorings of the truncated
 *          cube using at most n colors.
 * https://oeis.org/A274901
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = n^3*(n + 1)*(n^10 - n^9 + n^8 - n^7 + 7*n^6 - 4*n^5 +
 *            7*n^4 + 8*n^2 - 2*n + 8)/48
 * Range:     u8 n <= 1, u16 n <= 2, u32 n <= 6, u64 n <= 19, u128 n <= 19,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* use 128‑bit arithmetic to avoid overflow for 64‑bit seq_t values (the compiler must support
     * __uint128_t; this works on GCC/Clang and on many other compilers that provide the 128‑bit type ). */
    __uint128_t N  = n;
    __uint128_t N2 = N * N;
    __uint128_t N3 = N2 * N;
    __uint128_t N4 = N2 * N2;
    __uint128_t N5 = N4 * N;
    __uint128_t N6 = N3 * N3;
    __uint128_t N7 = N6 * N;
    __uint128_t N8 = N4 * N4;
    __uint128_t N9 = N8 * N;
    __uint128_t N10 = N5 * N5;

    __uint128_t P = N10 - N9 + N8 - N7 + 7 * N6 - 4 * N5 + 7 * N4 + 8 * N2 - 2 * N + 8;

    __uint128_t numerator = N3 * (N + 1) * P;
    return (seq_t)(numerator / 48);
}
