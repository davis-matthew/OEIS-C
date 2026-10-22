/*
 * A322084  Square array A(n,k), n >= 1, k >= 0, read by antidiagonals: A(n,k)
 *          = Sum_{d|n, n/d==1 (mod 4)} d^k - Sum_{d|n, n/d==3 (mod 4)} d^k.
 * https://oeis.org/A322084
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   not stated
 * Range:     u8 n <= 31, u16 n <= 69, u32 n <= 175, u64 n <= 441,
 *            u128 n <= 1139, big any n
 * Helpers:   diagonal
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*
 * Formula: a(n) = sum_{k} A(n',k') read by antidiagonals,
 * where A(n,k) = Σ_{d|n, n/d≡1 (mod 4)} d^k – Σ_{d|n, n/d≡3 (mod 4)} d^k
 * (n ≥ 1, k ≥ 0, indices read by antidiagonals).
 *
 * Implementation details:
 *   • Diagonal number D is the smallest integer with 1+…+D ≥ n.
 *   • For element D–pos_in_diag the row index is pos_in_diag,
 *     the column index is D–pos_in_diag.
 *   • A(n,k) is computed by iterating over divisors of n (up to √n) and
 *     adding or subtracting d^k according to the quadratic residue of the
 *     corresponding co‑divisor.
 *   • A small binary‑exponentiation routine ``pow_seq`` is supplied in
 *     case the utility function is not available.
 *
 * All operations are performed with the types ``seq_t`` and ``idx_t``
 * provided by the build system.  No floating point or external
 * libraries are used.
 */

#include <stdint.h>
#include <stddef.h>
#include "../utils/diagonal.h"

/* ------------------------------------------------------------------ */
/* Small binary exponentiation – works for any seq_t that supports  * */
/*   binary operators.                                               */
static seq_t pow_seq(seq_t base, idx_t exp)
{
    seq_t result = 1;
    seq_t b = base;
    while (exp > (idx_t)0) {
        if (exp & (idx_t)1)
            result *= b;
        exp >>= (idx_t)1;
        if (exp)
            b *= b;
    }
    return result;
}

/* ------------------------------------------------------------------ */
/* Diagonal number D: smallest D with D*(D+1)/2 >= n.                 */

/* ------------------------------------------------------------------ */
/* Compute A(n,k).  n >= 1, k >= 0.  All calculations are done in    */
/* ``seq_t``.                                                        */
static seq_t A_row_col(idx_t row, idx_t col)
{
    seq_t sum_pos = 0;
    seq_t sum_neg = 0;

    for (idx_t d = 1; d * d <= row; ++d) {
        if (row % d != (idx_t)0)
            continue;
        idx_t m = row / d;        /* co‑divisor */

        /* first divisor d, quotient m */
        if (m % 4 == (idx_t)1)
            sum_pos += pow_seq((seq_t)d, col);
        else if (m % 4 == (idx_t)3)
            sum_neg += pow_seq((seq_t)d, col);

        /* second divisor m, quotient d (if distinct) */
        if (d != m) {
            if (d % 4 == (idx_t)1)
                sum_pos += pow_seq((seq_t)m, col);
            else if (d % 4 == (idx_t)3)
                sum_neg += pow_seq((seq_t)m, col);
        }
    }
    return sum_pos - sum_neg;
}

/* ------------------------------------------------------------------ */
/* Public sequence function.  Returns a(n) for n ≥ 1.                 */
seq_t a(idx_t n)
{
    if (n == (idx_t)0)
        return 0;                /* safety – offset starts at 1 */

    idx_t D = diagonal(n);           /* diagonal number, 1 ≤ D ≤ ... */
    idx_t prev = (unsigned __int128)(D - (idx_t)1) * D / 2;
    idx_t pos_in_diag = n - prev;    /* 1‑based position on the diagonal */

    /* Map to (row, column) indices of the square array */
    idx_t row = pos_in_diag;             /* ≥ 1 */
    idx_t col = D - pos_in_diag;         /* ≥ 0 */

    return A_row_col(row, col);
}
