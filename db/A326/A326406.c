/*
 * A326406  Minesweeper sequence of positive integers arranged on a 2D grid
 *          along a triangular maze.
 * https://oeis.org/A326406
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  sign, tabl
 * Formula:   a(n) = -1 if n is prime; otherwise a(n) is the number of
 *            adjacent primes in the triangular maze arrangement of the
 *            positive integers
 * Range:     i8 n <= 11325, i16 n <= 11325, i32 n <= 11325, i64 n <= 11325,
 *            i128 n <= 11325, big any n
 * Validated: standard, pass, 94 terms through n = 9348 (2026-11-01)
 * Helpers:   is_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>
#include "../utils/is_prime.h"

/* Convert coordinates (row, col) back to the linear index n. */
static idx_t n_of_rc(idx_t row, idx_t col)
{
    idx_t s = row + col;                     /* diagonal index */
    idx_t base = s * (s + 1) / 2;
    if (s % 2 == 0)
        return base + col + 1;
    else
        return base + (s - col) + 1;
}

/* Convert a linear index n (1‑based) to its coordinates (row, col) in the
   triangular maze.  Row and column are 0‑based. */
static void rc_from_n(idx_t n, idx_t *row, idx_t *col)
{
    /* Find the diagonal s such that s*(s+1)/2 < n <= (s+1)*(s+2)/2. */
    idx_t low  = 0;
    idx_t high = 1;
    while (((high + 1) * (high + 2)) / 2 < n)
        high <<= 1;

    while (low < high) {
        idx_t mid = low + (high - low) / 2;
        if (((mid + 1) * (mid + 2)) / 2 >= n)
            high = mid;
        else
            low = mid + 1;
    }
    idx_t s = low;                       /* diagonal index */
    idx_t offset = n - s * (s + 1) / 2 - 1;   /* 0‑based offset on that diagonal */

    if (s % 2 == 0)          /* even diagonal: left → right */
        *col = offset;
    else                     /* odd diagonal: right → left */
        *col = s - offset;

    *row = s - *col;
}

/* 8-direction offsets in a rectangular grid. */
static const int dr[8] = {-1, -1, -1,  0, 0,  1, 1, 1};
static const int dc[8] = {-1,  0,  1, -1, 1, -1, 0, 1};

seq_t a(idx_t n)
{
    if (n == 0)            /* not defined by OEIS, just return 0 */
        return 0;

    if (is_prime(n))
        return (seq_t)-1;   /* primes are mines */

    idx_t row, col;
    rc_from_n(n, &row, &col);

    int count = 0;
    for (int k = 0; k < 8; ++k) {
        int64_t nr = (int64_t)row + dr[k];
        int64_t nc = (int64_t)col + dc[k];
        if (nr < 0 || nc < 0)          /* outside the infinite grid */
            continue;
        idx_t neigh = n_of_rc((idx_t)nr, (idx_t)nc);
        if (is_prime(neigh))
            ++count;
    }
    return (seq_t)count;
}
