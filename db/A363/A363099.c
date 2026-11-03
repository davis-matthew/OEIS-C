/*
 * A363099  Triangle T(n,k) in which the n-th row encodes the inverse of a
 *          3n+1 X 3n+1 Jacobi matrix, with 1's on the lower, main, and upper
 *          diagonals in GF(2), where the encoding consists of the decimal
 *          representations for the binary rows (n >= 1, 1 <= k <= 3n+1).
 * https://oeis.org/A363099
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base, tabf
 * Formula:   Triangle T(r,c) (r ≥ 1, 1 ≤ c ≤ 3r+1) is defined by • Base row
 *            1: T(1,1)=11, T(1,2)=3, T(1,3)=12, T(1,4)=13. • For rows r ≥ 2:
 *            – For 1 ≤ c ≤ 3r–2 if c % 3 ≠ 0 → T(r,c) = 8 * T(r‑1,c) + 3 if c
 *            % 3 = 0 → T(r,c) = 8 * T(r‑1,c) – T(r,3r‑1) = 3 – T(r,3r) =
 *            T(r,3r‑2) + 1 – T(r,3r+1) = T(r,3r) + 1 The s...
 * Range:     u8 n <= 11, u16 n <= 50, u32 n <= 175, u64 n <= 714,
 *            u128 n <= 1425, big any n
 * Validated: standard, pass, 72 terms through n = 1375 (2026-11-01)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* The types seq_t and idx_t are supplied by the build. */
#include <stddef.h>          /* for size_t */

/* Recursive helper to compute T(row, col).  The depth is at most the
   row number, which for n up to 10 000 is below 100. */
static inline seq_t T_helper(idx_t row, idx_t col)
{
    if (row == 1) {                     /* base row */
        switch (col) {
        case 1: return (seq_t)11;
        case 2: return (seq_t)3;
        case 3: return (seq_t)12;
        case 4: return (seq_t)13;
        default: return (seq_t)0;       /* not reached for valid input */
        }
    }

    seq_t three_r      = (seq_t)3 * (seq_t)row;
    seq_t three_r_minus1 = three_r - 1;
    seq_t three_r_minus2 = three_r - 2;

    if (col <= three_r_minus2) {                /* use previous row */
        if (col % 3 != 0)
            return ((seq_t)8 * T_helper(row - 1, col)) + (seq_t)3;
        else
            return ((seq_t)8 * T_helper(row - 1, col));
    } else if (col == three_r_minus1) {          /* special constant */
        return (seq_t)3;
    } else if (col == three_r) {                 /* shift by 2 */
        return T_helper(row, col - 2) + (seq_t)1;
    } else { /* col == three_r + 1 */            /* shift by 1 */
        return T_helper(row, col - 1) + (seq_t)1;
    }
}

/* Main entry point required by the benchmark harness. */
seq_t a(idx_t n)
{
    /* Find the row 'r' that contains the n-th term. */
    idx_t r = 1;
    while (1) {
        /* cum = sum_{i=1}^{r} (3i+1) = 3*r*(r+1)/2 + r */
        __int128 cum = (__int128)3 * r * (r + 1) / 2 + r;
        if (cum >= n) break;
        ++r;
    }

    /* Calculate how many terms precede this row. */
    __int128 prev_cum = (__int128)3 * (r - 1) * (r) / 2 + (r - 1);
    idx_t col = n - (idx_t)prev_cum;   /* 1‑based column in the row */

    return T_helper(r, col);
}
