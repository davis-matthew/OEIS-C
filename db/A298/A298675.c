/*
 * A298675  Rectangular array A: first differences of row entries of array
 *          A294099, read by antidiagonals.
 * https://oeis.org/A298675
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  sign, tabl
 * Formula:   not stated
 * Range:     i8 n <= 24, i16 n <= 60, i32 n <= 66, i64 n <= 66, i128 n <= 66,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  OEIS A298675
 *  a(n) enumerates the entries of the dilated Chebyshev array
 *  A294099 along antidiagonals.
 *
 *  For a given index n ≥ 1:
 *      1. Find the smallest integer d such that (d‑1)d/2 ≥ n.
 *      2. Let pos = n – (d‑2)(d‑1)/2   (1 ≤ pos ≤ d‑1).
 *      3. Map the pair (row, column) to
 *            row = d – pos
 *            col = pos
 *      4. Return T_col(row), where
 *            T₀(x) = 2,
 *            T₁(x) = x,
 *            T_j(x) = x·T_{j‑1}(x) – T_{j‑2}(x)   for j ≥ 2.
 *
 *  The computation uses only integer arithmetic; the routine works
 *  for any seq_t supported by the build (int8_t … __int128 or a
 *  big‑integer class).  It is O(√n) in time (≤≈150 steps for n=10⁴).
 */

seq_t a(idx_t n)
{
    /* Find diagonal d (sum of indices) and position pos within it */
    idx_t d = 2;                 /* current diagonal (sum i+j) */
    idx_t terms_up_to = 1;       /* total terms up to current diagonal */

    while (terms_up_to < n) {
        ++d;                     /* move to next diagonal */
        terms_up_to += d - 1;    /* add its length */
    }

    /* terms before this diagonal */
    idx_t terms_before = terms_up_to - (d - 1);
    idx_t pos = n - terms_before;     /* 1 .. d-1 */
    idx_t row = d - pos;              /* i  */
    idx_t col = pos;                  /* k  */

    /* Compute T_col(row) via the recurrence */
    if (col == 0)          /* never reached, but kept for completeness */
        return (seq_t)2;
    if (col == 1)
        return (seq_t)row;

    seq_t T_prevprev = (seq_t)2;      /* T_0(row) */
    seq_t T_prev     = (seq_t)row;    /* T_1(row) */
    seq_t T_curr;

    for (idx_t j = 2; j <= col; ++j) {
        T_curr = (seq_t)row * T_prev - T_prevprev;
        T_prevprev = T_prev;
        T_prev = T_curr;
    }

    return T_curr;
}
