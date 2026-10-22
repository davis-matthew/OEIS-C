/*
 * A343097  Array read by antidiagonals: T(n,k) is the number of k-colorings
 *          of an n X n grid, up to rotations and reflections.
 * https://oeis.org/A343097
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   a(n) = array read by antidiagonals of T(n,k) = (k^(n^2) +
 *            2*k^((n^2 + 3*(n mod 2))/4) + k^((n^2 + (n mod 2))/2) +
 *            2*k^(n*(n+1)/2) + 2*k^(n*(n + n mod 2)/2)) / 8
 * Range:     u8 n <= 17, u16 n <= 24, u32 n <= 39, u64 n <= 60, u128 n <= 98,
 *            big any n
 * Helpers:   big_pow
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/big_pow.h"

/* Integer square root for unsigned 64‑bit integers. */
static idx_t floor_isqrt(idx_t x)
{
    idx_t lo = 0;
    idx_t hi = (x >> 1) + 1;  /* Upper bound */
    while (lo < hi) {
        idx_t mid = (lo + hi) >> 1;
        unsigned __int128 sq = (unsigned __int128)mid * mid;
        if (sq <= x)
            lo = mid + 1;
        else
            hi = mid;
    }
    return lo - 1;
}

/* Compute T(n,k) from the OEIS formula. */
static seq_t compute_T(idx_t n_row, idx_t k_col)
{
    /* Cast to seq_t for the arithmetic. */
    seq_t n = n_row;
    seq_t k = k_col;

    seq_t e1 = n * n;
    seq_t nmod2 = n & 1U;

    seq_t e2 = (e1 + 3U * nmod2) / 4U;
    seq_t e3 = (e1 + nmod2) / 2U;
    seq_t e4 = n * (n + 1U) / 2U;
    seq_t e5 = n * (n + nmod2) / 2U;

    seq_t t1 = big_pow(k, e1);
    seq_t t2 = 2U * big_pow(k, e2);
    seq_t t3 = big_pow(k, e3);
    seq_t t4 = 2U * big_pow(k, e4);
    seq_t t5 = 2U * big_pow(k, e5);

    seq_t sum = t1 + t2 + t3 + t4 + t5;
    return sum / 8U;
}

/* Translate the one‑dimensional index to (row,col) on antidiagonals. */
static void index_to_coords(idx_t idx, idx_t *row, idx_t *col)
{
    /* Determine the diagonal number s. */
    idx_t t = (floor_isqrt(8U * idx + 1U) + 1U) >> 1;
    idx_t s = t - 1U;

    idx_t cum_before = (s * (s + 1U)) >> 1;   /* s*(s+1)/2 */
    idx_t offset = idx - cum_before;
    *row = offset;
    *col = s - offset;
}

/* External function required by the build. */
seq_t a(idx_t n)
{
    if (n == (idx_t)-1) return 0;   /* defensive, not needed in tests */

    idx_t i, j;
    index_to_coords(n, &i, &j);
    return compute_T(i, j);
}
