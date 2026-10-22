/*
 * A303465  Number of nX4 0..1 arrays with every element equal to 0, 1, 2, 3,
 *          5 or 6 horizontally, diagonally or antidiagonally adjacent
 *          elements, with upper left element zero.
 * https://oeis.org/A303465
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = number of n×4 binary arrays with the upper–left cell *
 *            zero, such that for every cell the number of horizontally,
 *            diagonally * or antidiagonally adjacent cells with the same
 *            value is in {0,1,2,3,5,6} * (i.e. exactly 4 equal neighbors are
 *            forbidden)
 * Range:     u8 n <= 2, u16 n <= 4, u32 n <= 9, u64 n <= 19, u128 n <= 38,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>
#include <stddef.h>
 
/* 16 possible 4‑bit rows, plus one special value for a non‑existent row. */
#define NO_ROW 16
#define ROWS   17
#define STATES (ROWS * ROWS)    /* 289 */
 
/* Check whether middle row 'b' is valid given its upper neighbour 'a'
 * and lower neighbour 'c'.  'NO_ROW' indicates a missing neighbour row.
 * The function forbids a cell having exactly four equal neighbours. */
static inline bool row_valid(uint8_t a, uint8_t b, uint8_t c)
{
    for (int col = 0; col < 4; ++col) {
        uint8_t v = (b >> col) & 1;
        int cnt = 0;

        /* left / right neighbours in the same row */
        if (col > 0   && ((b >> (col - 1)) & 1) == v) ++cnt;
        if (col < 3   && ((b >> (col + 1)) & 1) == v) ++cnt;

        /* diagonal neighbours from the row above (if it exists) */
        if (a != NO_ROW) {
            if (col > 0   && ((a >> (col - 1)) & 1) == v) ++cnt;
            if (col < 3   && ((a >> (col + 1)) & 1) == v) ++cnt;
        }

        /* diagonal neighbours from the row below (if it exists) */
        if (c != NO_ROW) {
            if (col > 0   && ((c >> (col - 1)) & 1) == v) ++cnt;
            if (col < 3   && ((c >> (col + 1)) & 1) == v) ++cnt;
        }

        if (cnt == 4) return false;   /* forbidden configuration */
    }
    return true;
}
 
seq_t a(idx_t n)
{
    if (n == 0) return 0;          /* undefined below the offset */
 
    seq_t dp[STATES]  = {0};
    seq_t next[STATES] = {0};
 
    /* initial state: two non‑existent rows */
    dp[NO_ROW * ROWS + NO_ROW] = 1;
 
    for (idx_t step = 0; step < n; ++step) {
        /* zero initialise next */
        for (size_t i = 0; i < STATES; ++i) next[i] = 0;
 
        for (size_t a = 0; a < ROWS; ++a) {
            for (size_t b = 0; b < ROWS; ++b) {
                seq_t cur = dp[a * ROWS + b];
                if (cur == 0) continue;
 
                for (uint8_t c = 0; c < 16; ++c) {
                    /* upper‑left element must be 0 in the first row */
                    if (step == 0 && (c & 1)) continue;
                    /* validate the middle row only after it exists */
                    if (step > 0 && !row_valid((uint8_t)a, (uint8_t)b, c))
                        continue;
                    next[b * ROWS + c] += cur;
                }
            }
        }
        /* move next to dp for the next iteration */
        for (size_t i = 0; i < STATES; ++i) dp[i] = next[i];
    }
 
    /* after all rows, validate the last row (no row below) */
    seq_t result = 0;
    for (size_t a = 0; a < ROWS; ++a) {
        for (size_t b = 0; b < ROWS; ++b) {
            seq_t cur = dp[a * ROWS + b];
            if (cur == 0) continue;
            if (row_valid((uint8_t)a, (uint8_t)b, NO_ROW))
                result += cur;
        }
    }
 
    return result;
}
