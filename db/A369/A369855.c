/*
 * A369855  a(n) is the smallest error in trying to solve n^5 = x^5 + y^5: for
 *          each n from 2 on, find positive integers x and y, x <= y < n such
 *          that |n^5 - x^5 - y^5| is minimal and let a(n) = n^5 - x^5 - y^5.
 *          In case of a tie, choose the solution with smallest y.
 * https://oeis.org/A369855
 *
 * Offset:    2    (a(2) is the first term)
 * Keywords:  sign
 * Formula:   not stated
 * Range:     i8 n <= 2, i16 n <= 9, i32 n <= 41, i64 n <= 41, i128 n <= 41,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* OEIS A369855 – a(n) = minimal n^5 – x^5 – y^5 for positive integers
   1 ≤ x ≤ y < n, choosing the pair with minimal |n^5 – x^5 – y^5| and, on a tie,
   the smallest y.  The function returns a(n) = n^5 – x^5 – y^5 for each n ≥ 2. */

#include <stdbool.h>

/* ----------  auxiliary math helpers  ---------- */

/* compute v^5 with integer arithmetic */
static inline seq_t pow5(seq_t v)
{
    seq_t p = v;
    p *= v;   /* v^2 */
    p *= v;   /* v^3 */
    p *= v;   /* v^4 */
    p *= v;   /* v^5 */
    return p;
}

/* largest integer x (1 ≤ x ≤ maxx) with x^5 ≤ rem, or 0 if none */
static inline idx_t floor_root5(seq_t rem, idx_t maxx)
{
    idx_t lo = 1, hi = maxx, best = 0;
    while (lo <= hi) {
        idx_t mid = lo + (hi - lo) / 2;
        seq_t val = pow5((seq_t)mid);
        if (val <= rem) {
            best = mid;
            lo = mid + 1;
        } else {
            hi = mid - 1;
        }
    }
    return best;  /* 0 indicates no fifth power ≤ rem within [1,maxx] */
}

/* ----------  sequence a(n)  ---------- */
seq_t a(idx_t n)
{
    if (n < 2) return 0;        /* undefined for n < 2 */

    const seq_t n5 = pow5((seq_t)n);

    seq_t best_diff = 0;
    seq_t best_abs  = 0;
    idx_t best_y    = 0;
    bool first = true;

    for (idx_t y = n - 1; y > 0; --y) {
        const seq_t y5 = pow5((seq_t)y);
        const seq_t rem = n5 - y5;                /* remaining part for x^5 */

        /* candidate with x = floor_root5(rem, y) (will be ≤ y) */
        idx_t x_floor = floor_root5(rem, y);
        if (x_floor >= 1) {                       /* x must be ≥ 1 */
            const seq_t diff1 = n5 - pow5((seq_t)x_floor) - y5;
            const seq_t abs1  = diff1 >= 0 ? diff1 : -diff1;
            if (first || abs1 < best_abs ||
               (abs1 == best_abs && y < best_y)) {
                best_abs  = abs1;
                best_diff = diff1;
                best_y    = y;
                first = false;
            }
        }

        /* candidate with x = x_floor + 1 (if within bounds) */
        idx_t x_ceil = x_floor + 1;
        if (x_ceil <= y) {
            const seq_t diff2 = n5 - pow5((seq_t)x_ceil) - y5;
            const seq_t abs2  = diff2 >= 0 ? diff2 : -diff2;
            if (first || abs2 < best_abs ||
               (abs2 == best_abs && y < best_y)) {
                best_abs  = abs2;
                best_diff = diff2;
                best_y    = y;
                first = false;
            }
        }
    }

    return best_diff;
}
