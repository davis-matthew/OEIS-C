/*
 * A274079  Table read by rows: the n-th row is the list of numbers diagonally
 *          up and to the right of n in the natural numbers read by
 *          antidiagonals.
 * https://oeis.org/A274079
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, tabf
 * Formula:   not stated
 * Range:     u8 n <= 1774, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A274079
   Table read by rows: the n‑th row is the list of numbers
   diagonally up and to the right of n in the natural numbers
   read by antidiagonals.
   The flattened sequence a(n) (offset 1) is returned by this function.
   Computation:
     * For each natural number m ≥ 1, find its position (r,c) in the
       antidiagonal layout of the natural numbers.
       The position satisfies:
           t = r + c – 2              (t is the antidiagonal index)
           m = t·(t+1)/2 + r          (m is the natural number at (r,c))
     * The row for m consists of the r‑1 numbers at
           (r‑Δ , c+Δ)   for   Δ = 1 … r‑1
     * The flat sequence a(n) is obtained by concatenating these rows
       for m = 1, 2, 3, … in increasing order.
*/

#include <stdint.h>

static inline idx_t cumulative(idx_t t) /* (t+1)*(t+2)/2 */
{
    return (idx_t)(((__uint128_t)(t + 1) * (t + 2)) / 2);
}

static inline seq_t value_at(idx_t r, idx_t c) /* returns the natural number at (r,c) */
{
    idx_t sum_rc = r + c - 2;                      /* t = r + c - 2 */
    return (seq_t)(((__uint128_t)sum_rc * (sum_rc + 1)) / 2 + r);
}

seq_t a(idx_t n)
{
    if (n == 0) return 0;            /* not used – offset is 1 */

    idx_t m = 1;                     /* current natural number (row index) */
    idx_t t = 0;                     /* current antidiagonal index */
    idx_t cum = cumulative(t);       /* cumulative count up to diagonal t */
    idx_t consumed = 0;              /* number of elements already produced */

    for (;; ++m) {
        /* advance t until m lies within the current diagonal t */
        while (m > cum) {
            ++t;
            cum = cumulative(t);
        }

        /* previous cumulative count (up to diagonal t‑1) */
        idx_t prev = (idx_t)(((__uint128_t)t * (t + 1)) / 2);

        idx_t r = m - prev;          /* row index of m */
        idx_t c = t + 2 - r;          /* column index of m */

        idx_t row_len = (r > 0) ? r - 1 : 0;
        if (row_len == 0) continue;  /* empty row – skip */

        if (consumed + row_len >= n) {
            idx_t offset = n - consumed - 1;     /* 0‑based offset within the row */
            idx_t r_prime = r - (offset + 1);
            idx_t c_prime = c + (offset + 1);
            return value_at(r_prime, c_prime);
        }

        consumed += row_len;          /* advance the count */
    }
}
