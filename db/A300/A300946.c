/*
 * A300946  Rectangular array A(n, k) = (-1)^k*hypergeom([-k, k + n/2 - 1/2],
 *          [1], 4) with row n >= 0 and k >= 0, read by ascending
 *          antidiagonals.
 * https://oeis.org/A300946
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   a(n) is given by the antidiagonal reading of * A(n,k) = (-1)^k *
 *            hypergeom([-k, k + n/2 - 1/2], [1], 4) (n,k ≥ 0)
 * Range:     u8 n <= 8, u16 n <= 13, u32 n <= 43, u64 n <= 44, u128 n <= 44,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* The function uses an exact recurrence derived from the
 * summation formula and avoids floating‑point arithmetic. */

#include <stdint.h>
#include <stdbool.h>

/* --------------------------------------------------------------------
 * Integer square root (floor) for 64‑bit unsigned integers.
 * -------------------------------------------------------------------- */
static inline idx_t isqrt_u64(idx_t x)
{
    idx_t lo = 0;
    idx_t hi = x;
    while (lo < hi) {
        idx_t mid = lo + (hi - lo + 1) / 2;
        if (mid > 0 && mid > x / mid)   /* mid * mid would overflow */
            hi = mid - 1;
        else
            lo = mid;
    }
    return lo;
}

/* --------------------------------------------------------------------
 * Compute A(n,k) for arbitrary n,k ≥ 0 using an exact recurrence.
 * -------------------------------------------------------------------- */
static seq_t A_value(idx_t row, idx_t k)
{
    /* Base term: j = 0  -> magnitude 1, sign = (-1)^k */
    seq_t mag = 1;
    int sign = (k & 1) ? -1 : 1;        /* (-1)^k  */
    seq_t sum_pos = 0, sum_neg = 0;
    if (sign > 0) sum_pos = mag;
    else          sum_neg = mag;

    /* Recurrence:
     *   mag_{j+1} = mag_j * (k - j) * (4*(k + j) + 2*row - 2) / ((j+1)^2)
     *   and the sign flips each step.
     */
    for (idx_t j = 0; j < k; ++j) {
        seq_t num1 = (seq_t)(k - j);
        seq_t num2 = (seq_t)(4 * (k + j) + 2 * row - 2);
        seq_t denom = (seq_t)((j + 1) * (j + 1));

        /* Update magnitude */
        mag *= num1;
        mag *= num2;
        mag /= denom;                 /* exact division */

        sign = -sign;                 /* flip sign */

        if (sign > 0) sum_pos += mag;
        else          sum_neg += mag;
    }

    /* Result is the difference of the positive and negative sums */
    return sum_pos - sum_neg;
}

/* --------------------------------------------------------------------
 * Main sequence function: a(n) – antidiagonal reading of the array.
 * -------------------------------------------------------------------- */
seq_t a(idx_t n)
{
    /* Determine coordinates (row, col) from anti‑diagonal index n. */
    idx_t s = (isqrt_u64((idx_t)((unsigned __int128)n * 8 + 1)) - 1) / 2;

    /* Correct possible rounding errors */
    while ((s + 1) * (s + 2) / 2 <= n) ++s;
    while (s * (s + 1) / 2 > n) --s;

    idx_t d   = n - s * (s + 1) / 2;   /* offset inside the diagonal   */
    idx_t row = s - d;                 /* n‑coordinate of the entry    */
    idx_t col = d;                     /* k‑coordinate of the entry    */

    return A_value(row, col);
}
