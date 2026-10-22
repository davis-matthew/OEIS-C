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
 * Range:     i8 n <= 9348, i16 n <= 9348, i32 n <= 9348, i64 n <= 9348,
 *            i128 n <= 9348, big any n
 * Helpers:   is_prime, n_of_rc, rc_from_n
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>
#include "../utils/is_prime.h"
#include "../utils/n_of_rc.h"
#include "../utils/rc_from_n.h"

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
