/*
 * A326405  Minesweeper sequence of positive integers arranged on a 2D grid
 *          along ascending antidiagonals.
 * https://oeis.org/A326405
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  sign, tabl
 * Formula:   a(n) = -1 if n is prime; otherwise the number of primes in the 8
 *            adjacent cells of the 2‑D antidiagonal grid
 * Range:     i8 n <= 9805, i16 n <= 9805, i32 n <= 9805, i64 n <= 9805,
 *            i128 n <= 9805, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>

/* ------------------------------------------------------------------ */
/* Helper: test primality by trial division (fast enough for < 2^16).   */
static inline bool is_prime_uint64(idx_t n)
{
    if (n < 2) return false;
    if (n % 2 == 0) return n == 2;
    if (n % 3 == 0) return n == 3;
    for (idx_t d = 5; d * d <= n; d += 6) {
        if (n % d == 0 || n % (d + 2) == 0) return false;
    }
    return true;
}

/* ------------------------------------------------------------------ */
/* Triangular number T(k) = k*(k+1)/2.                                   */
static inline idx_t tri(idx_t k)
{
    return k * (k + 1) / 2;
}

/* ------------------------------------------------------------------ */
/* Convert 0‑based grid coordinates (row, col) to the grid label n.      */
static inline idx_t label_from_coords(idx_t row, idx_t col)
{
    return tri(row + col) + row + 1;
}

/* ------------------------------------------------------------------ */
/* Determine the 0‑based grid coordinates (row, col) for label n.          */
static inline void coords_from_label(idx_t n, idx_t *row, idx_t *col)
{
    idx_t t = 0;
    while (tri(t + 1) < n) ++t;              /* find the antidiagonal t */
    *row = n - 1 - tri(t);                  /* i = n‑1‑T(t)          */
    *col = t - *row;                        /* j = t‑i               */
}

/* ------------------------------------------------------------------ */
/* Main sequence function.                                               */
seq_t a(idx_t n)
{
    idx_t i, j;
    coords_from_label(n, &i, &j);

    if (is_prime_uint64(n))
        return (seq_t)(-1);                  /* mine */

    int count = 0;
    for (int di = -1; di <= 1; ++di) {
        for (int dj = -1; dj <= 1; ++dj) {
            if (di == 0 && dj == 0) continue;
            int64_t ni = (int64_t)i + di;
            int64_t nj = (int64_t)j + dj;
            if (ni < 0 || nj < 0) continue;
            idx_t nr = (idx_t)ni;
            idx_t nc = (idx_t)nj;
            idx_t m = label_from_coords(nr, nc);
            if (is_prime_uint64(m)) ++count;
        }
    }
    return (seq_t)count;
}
