/*
 * A278112  Triangle T(n,k) = floor(n sqrt(2/k)) for 1 <= k <= 2n^2, read by
 *          rows.
 * https://oeis.org/A278112
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, tabf, easy
 * Formula:   not stated
 * Range:     u8 n <= 2568, u16 n <= 9730, u32 n <= 9730, u64 n <= 9730,
 *            u128 n <= 9730, big any n
 * Helpers:   floor_sqrt
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* 
 * Sequence A278112: Triangle T(n,k) = floor(n * sqrt(2/k)),
 * 1 <= k <= 2n^2, read by rows.
 * a(n) is the term at index n in this flattened triangle.
 * For an index n we locate the row r such that
 *   S(r-1) < n <= S(r), where S(t) = t(t+1)(2t+1)/3
 * (the cumulative number of terms up to row t).
 * Then k = n - S(r-1) and the value is 
 *   a(n) = floor( sqrt( 2·r² / k ) ).
 * The floor of the square root is computed with floor_sqrt.
 */

#include "../utils/floor_sqrt.h"

seq_t a(idx_t n) {
    /* locate the row containing index n */
    idx_t r = 1;            /* current row number (1‑based)   */
    idx_t sum_prev = 0;     /* cumulative terms before row r */

    /* iterate until n fits in the current row */
    while (1) {
        idx_t row_terms = 2 * r * r;          /* terms in row r         */
        if (n <= sum_prev + row_terms) {      /* found the row           */
            idx_t k = n - sum_prev;           /* index within the row   */
            /* compute floor( sqrt( 2·r² / k ) ) */
            seq_t val = ((seq_t)2 * (seq_t)r * (seq_t)r) / (seq_t)k;
            return floor_sqrt(val);
        }
        sum_prev += row_terms;                /* advance to next row    */
        ++r;
    }
}
