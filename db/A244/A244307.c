/*
 * A244307  Sum over each antidiagonal of A244306.
 * https://oeis.org/A244307
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = (6*n^5 + 30*n^4 + 180*n^3 + 240*n^2 - 141*n - 135 + (45*n
 *            + 135)*(-1)^n)/1440
 * Range:     u8 n <= 7, u16 n <= 26, u32 n <= 251, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

extern seq_t a(idx_t n)
{
    /* Use unsigned 128‑bit intermediate to avoid overflow for 64‑bit seq_t. */
    unsigned __int128 N = n;
    unsigned __int128 numerator;

    /* Compute the polynomial parts that are always non‑negative first. */
    numerator = 6ULL * N * N * N * N * N
              + 30ULL * N * N * N * N
              + 180ULL * N * N * N
              + 240ULL * N * N;

    /* Subtract the smaller terms. */
    numerator -= 141ULL * N;
    numerator -= 135ULL;

    /* Add the alternating part. */
    unsigned __int128 alt = 45ULL * N + 135ULL;
    if ((n & 1ULL) != 0)          /* (-1)^n == -1 for odd n */
        numerator -= alt;
    else
        numerator += alt;

    /* Final division. */
    unsigned __int128 res = numerator / 1440ULL;
    return (seq_t)res;
}
