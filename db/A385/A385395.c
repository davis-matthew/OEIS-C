/*
 * A385395  Triangle read by rows: T(n, k) = [A047999(n, k) = 1 or A385456(n,
 *          k) = 1], where [.] is the Iverson bracket.
 * https://oeis.org/A385395
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   a(n) = 1 if and only if (row & col == col) or (row % 3 >= col %
 *            3 and (~(row/3) & (col/3)) == 0), otherwise 0. The index n
 *            (0‑based) is mapped to the unique triangle entry T(row, col) by
 *            reading the triangle row by row
 * Range:     u8 n <= 90, u16 n <= 90, u32 n <= 90, u64 n <= 90, u128 n <= 90,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>

/* Find the row r such that r*(r+1)/2 <= n < (r+1)*(r+2)/2.
   Binary search over 64‑bit unsigned integers with __int128 for safety. */
static inline uint64_t triangle_row(uint64_t n) {
    uint64_t low = 0, high = 1;
    /* Expand high until it exceeds the needed row index */
    while ((unsigned __int128)high * (high + 1) / 2 <= n) high <<= 1;
    /* Binary search in [low, high) */
    while (low < high) {
        uint64_t mid = low + (high - low) / 2;
        if ((unsigned __int128)mid * (mid + 1) / 2 <= n)
            low = mid + 1;
        else
            high = mid;
    }
    return low - 1;
}

/* Evaluate the triangle entry for a given (row, col). */
static inline seq_t triangle_value(uint64_t row, uint64_t col) {
    bool cond1 = (row & col) == col;
    bool cond2 = false;
    if ((row % 3) >= (col % 3))
        cond2 = ((~(row / 3)) & (col / 3)) == 0;
    return (seq_t)(cond1 || cond2);
}

/* External function required by the build. */
seq_t a(idx_t n) {
    uint64_t row = triangle_row((uint64_t)n);
    uint64_t before = (unsigned __int128)row * (row + 1) / 2; /* elements before this row */
    uint64_t col  = (uint64_t)n - before;
    return triangle_value(row, col);
}
