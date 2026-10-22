/*
 * A376778  a(n) = (32*n^4 + 80*n^3 + 40*n^2 - 20*n + 3)*(2*n + 1)*n/15.
 * https://oeis.org/A376778
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = (32*n^4 + 80*n^3 + 40*n^2 - 20*n + 3)*(2*n + 1)*n / 15
 * Range:     u8 n <= 1, u16 n <= 4, u32 n <= 31, u64 n <= 1275,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n)
{
    if (n == 0)
        return 0;

    /* use 128‑bit arithmetic to avoid overflow for 64‑bit seq_t */
    unsigned __int128 N  = (unsigned __int128)n;
    unsigned __int128 n2 = N * N;
    unsigned __int128 n3 = n2 * N;
    unsigned __int128 n4 = n3 * N;

    unsigned __int128 poly = 32 * n4 + 80 * n3 + 40 * n2 - 20 * N + 3;
    unsigned __int128 prod = poly * (2 * N + 1) * N;

    prod /= 15;

    return (seq_t)prod;
}
