/*
 * A373193  On a unit square grid, the number of squares enclosed by a circle
 *          of radius n with origin at the center of a square.
 * https://oeis.org/A373193
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 4*Sum_{k=1..n-1} floor(sqrt(n^2-(k+1/2)^2)-1/2) + 4*n - 3
 * Range:     u8 n <= 9, u16 n <= 145, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

/* Integer square root for unsigned __int128 values. */
static idx_t isqrt128(unsigned __int128 x)
{
    idx_t r = 0;
    idx_t bit = (idx_t)(1ULL << 63); /* highest bit for 64‑bit numbers */
    while (bit) {
        idx_t nxt = r + bit;
        if ((unsigned __int128)nxt * nxt <= x) {
            r = nxt;
        }
        bit >>= 1;
    }
    return r;
}

seq_t a(idx_t n)
{
    if (n < 1) return 0;          /* behaviour below the offset is irrelevant */
    if (n == 1) return 1;         /* shortcut – the formula also yields 1 */

    seq_t sum = 0;
    unsigned __int128 four_n_square = (unsigned __int128)4 * (unsigned __int128)n * (unsigned __int128)n;

    for (idx_t k = 1; k < n; ++k) {
        unsigned __int128 kk = (unsigned __int128)(2 * k + 1);
        unsigned __int128 B = four_n_square - kk * kk;      /* B > 0 for all 1 ≤ k < n */
        idx_t sqrtB = isqrt128(B);                          /* floor(sqrt(B)) */
        idx_t term = (sqrtB > 0) ? (sqrtB - 1) / 2 : 0;    /* floor((sqrtB−1)/2) */
        sum += (seq_t)term;
    }

    seq_t result = 4 * sum + (seq_t)4 * n - 3;
    return result;
}
