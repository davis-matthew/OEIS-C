/*
 * A309432  Number of distinct digits in decimal representation of n^2.
 * https://oeis.org/A309432
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, base, easy
 * Formula:   a(n) = number of distinct decimal digits in n^2
 * Range:     u8 n <= 10000, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>

seq_t a(idx_t n)
{
    /* Use 128‑bit arithmetic to avoid overflow of n^2. */
    unsigned __int128 sq = (unsigned __int128)n * n;
    if (sq == 0)
        return (seq_t)1;           /* 0^2 = 0 has one distinct digit */

    unsigned short mask = 0;       /* bitmask for digits 0–9 */
    while (sq > 0) {
        unsigned d = (unsigned)(sq % 10);  /* extract least‑significant digit */
        mask |= 1u << d;
        sq  /= 10;
    }

    /* Count set bits in mask → number of distinct digits. */
    unsigned cnt = 0;
    while (mask) {
        mask &= mask - 1;
        ++cnt;
    }
    return (seq_t)cnt;
}
