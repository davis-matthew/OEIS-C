/*
 * A337713  Irregular triangle T read by rows: row n gives the inverse
 *          elements of row n of A216319 Modd(n), for n >= 1.
 * https://oeis.org/A337713
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, tabf, easy
 * Formula:   a(n) is the n-th element of the irregular triangle T read by
 *            rows. Row k consists of the inverses (under Modd k) of the odd
 *            numbers < k that are coprime to k (the RRSodd(k) of A216319).
 *            The sequence is obtained by flattening the triangle.
 *            Implementation computes the required row on demand and the...
 * Range:     u8 n <= 103, u16 n <= 103, u32 n <= 103, u64 n <= 103,
 *            u128 n <= 103, big any n
 * Helpers:   inverse_modd
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>
#include "../utils/inverse_modd.h"

static inline idx_t ugcd(idx_t a, idx_t b)
{
    while (b) {
        idx_t t = a % b;
        a = b;
        b = t;
    }
    return a;
}

/* Count the number of odd integers < n that are coprime to n */
static inline idx_t delta_row(idx_t n)
{
    if (n <= 1) return 1;
    idx_t count = 0;
    for (idx_t j = 1; j < n; j += 2) {
        if (ugcd(j, n) == 1) ++count;
    }
    return count;
}

seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0;     /* not used, but safe */
    if (n == 1) return (seq_t)1;

    idx_t cum = 0;      /* total terms processed so far */
    idx_t row = 1;      /* current row number */

    /* Find the row that contains the n-th term */
    while (true) {
        idx_t d = delta_row(row);
        if (cum + d >= n) break;
        cum += d;
        ++row;
    }

    if (row == 1) {        /* special case for row 1 */
        return (seq_t)1;
    }

    /* Column (1‑based) within the found row */
    idx_t col = n - cum;

    /* Find the col‑th odd coprime element and its inverse */
    idx_t count = 0;
    for (idx_t j = 1; j < row; j += 2) {
        if (ugcd(j, row) != 1) continue;
        ++count;
        if (count == col) {
            idx_t inv = inverse_modd(j, row);
            return (seq_t)inv;
        }
    }

    /* Fallback – should not occur */
    return (seq_t)1;
}
