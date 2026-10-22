/*
 * A329940  Square array read by antidiagonals upwards: T(n,k) is the number
 *          of right unique relations between set A with n elements and set B
 *          with k elements.
 * https://oeis.org/A329940
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, tabl, easy
 * Formula:   a(n) = (k + 1)^n - 1, where (n,k) are the indices of the table
 *            T(n,k) = (k + 1)^n - 1 read along antidiagonals from the lowest
 *            row to the highest row
 * Range:     u8 n <= 22, u16 n <= 48, u32 n <= 55, u64 n <= 55, u128 n <= 55,
 *            big any n
 * Helpers:   big_pow, min_diag
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Utility needed for integer‑powered arithmetic */
#include <stdint.h>
#include "../utils/big_pow.h"
#include "../utils/min_diag.h"

seq_t a(idx_t n)
{
    if (n == 0)
        return 0;                /* offset starts at 1 */

    /* Determine the diagonal on which the index n lies. */
    idx_t s = min_diag(n);

    /* Total number of entries before this diagonal. */
    idx_t before = (idx_t)(((__int128)(s - 2) * (s - 1)) / 2);

    /* Zero‑based position within the diagonal. */
    idx_t pos = n - before - 1;

    /* Convert that position into table coordinates. */
    idx_t row = (s - 1) - pos;   /* n‑index (1 ≤ row) */
    idx_t col = pos + 1;        /* k‑index (1 ≤ col) */

    /* Compute T(row, col) = (col + 1)^row - 1. */
    seq_t base = (seq_t)(col + 1);
    seq_t exp  = (seq_t)row;

    seq_t val = big_pow(base, exp);
    return val - 1;
}
