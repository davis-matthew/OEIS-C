/*
 * A319120  T(n, k) = binomial(n - k - 1, k)*binomial(2*n - 2*k, n)/(n + 1),
 *          for n >= 1 and 0 <= k <= floor((n - 1)/2), triangle read by rows.
 * https://oeis.org/A319120
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, tabf, nonn
 * Formula:   a(n) is the (flattened) triangle entry T(row, k) of T(n, k) =
 *            binomial(n - k - 1, k) * binomial(2*n - 2*k, n) / (n + 1) for n
 *            ≥ 1 and 0 ≤ k ≤ ⌊(n - 1)/2⌋. The flattening uses the standard
 *            OEIS convention: row 1: a(1) row 2: a(2) row 3: a(3), a(4) row
 *            4: a(5), a(6) and so on
 * Range:     u8 n <= 4, u16 n <= 16, u32 n <= 42, u64 n <= 42, u128 n <= 42,
 *            big any n
 * Helpers:   binomial, gcd
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/binomial.h"   /* seq_t binomial(idx_t a, idx_t b); */
#include "../utils/gcd.h"        /* seq_t gcd(seq_t a, seq_t b); */

seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0;

    /* Determine the row and column of the flattened triangle */
    idx_t row = 1;
    idx_t cnt = 0;          /* number of terms seen so far */
    while (1) {
        idx_t len = (row & 1) ? ((row + 1) >> 1) : (row >> 1); /* row length */
        if (cnt + len >= n) break;
        cnt += len;
        ++row;
    }
    idx_t k = n - cnt - 1;  /* column index within this row (0‑based) */

    /* Compute the two binomial factors */
    seq_t a1 = binomial(row - k - 1, k);
    idx_t two_n_minus_2k = (row << 1) - (k << 1);
    seq_t a2 = binomial(two_n_minus_2k, row);

    /* Reduce before multiplying to avoid overflow in the 64‑bit build
       when the final result fits but the raw product does not. */
    seq_t d = gcd((seq_t)(row + 1), a1);
    a1 /= d;
    seq_t rem = (seq_t)(row + 1) / d;
    d = gcd(rem, a2);
    a2 /= d;

    /* rem should be 1 after the reductions */
    (void)rem;  /* silence unused warning */

    seq_t result = a1 * a2; /* exact division by (row+1) has been handled */
    return result;
}
