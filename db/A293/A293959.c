/*
 * A293959  Construct a triangle T(n,k) (0 <= k <= n) of strings of integers,
 *          where T(0,0) = {0}, T(n,n) = {n}, and otherwise T(n,k) is the
 *          concatenation of T(n-1,k-1) and T(n-1,k). The sequence is obtained
 *          by reading across the rows of the triangle, concatenating the
 *          successive strings.
 * https://oeis.org/A293959
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, tabf
 * Formula:   not stated
 * Range:     u8 n <= 119, u16 n <= 119, u32 n <= 119, u64 n <= 119,
 *            u128 n <= 119, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computes A293959: the nth digit of the concatenation of the binary triangle strings
   defined in the OEIS entry. */

#include <stdint.h>
#include <stdbool.h>

/* The types seq_t and idx_t are defined by the build system. */

#define MAX_ROWS 64          /* enough for n ≤ 10 000 */

static inline idx_t digits_of(idx_t n) {
    idx_t d = 0;
    do { d++; n /= 10; } while (n);
    return d;
}

static inline seq_t digit_of_number(idx_t number, idx_t pos) {
    idx_t len = digits_of(number);
    idx_t exp = len - 1 - pos;
    idx_t pow10 = 1;
    for (idx_t i = 0; i < exp; ++i) pow10 *= 10;
    return (seq_t)((number / pow10) % 10);
}

/* Recursive routine to fetch the digit at position 'pos' inside T(row,k). */
static seq_t get_digit(idx_t row, idx_t k, idx_t pos,
                       idx_t row_len[MAX_ROWS][MAX_ROWS]) {
    if (row == 0)                     /* T(0,0) = "0" */
        return (seq_t)0;

    if (k == 0)                        /* T(row,0) = "0"         */
        return (seq_t)0;

    if (k == row)                      /* T(row,row) = decimal(row) */
        return digit_of_number(row, pos);

    /* internal entry: concatenate T(row-1,k-1) and T(row-1,k)   */
    idx_t left_len = row_len[row - 1][k - 1];
    if (pos < left_len)
        return get_digit(row - 1, k - 1, pos, row_len);
    else
        return get_digit(row - 1, k, pos - left_len, row_len);
}

/* Public entry point for the sequence. */
seq_t a(idx_t n) {
    idx_t row_len[MAX_ROWS][MAX_ROWS] = {{0}};
    idx_t cumulative = 0;   /* total digits processed so far      */
    idx_t target_row = 0;   /* row that contains the nth digit    */
    idx_t offset = 0;       /* position inside the target row     */

    /* Build rows until we reach the row that contains position n. */
    for (idx_t row = 0; ; ++row) {
        idx_t row_total = 0;
        if (row == 0) {
            row_len[row][0] = 1;   /* T(0,0) = "0"          */
            row_total = 1;
        } else {
            row_len[row][0] = 1;   /* T(row,0) = "0"        */
            for (idx_t k = 1; k < row; ++k)
                row_len[row][k] = row_len[row - 1][k - 1] + row_len[row - 1][k];
            row_len[row][row] = digits_of(row);   /* T(row,row)           */
            for (idx_t k = 0; k <= row; ++k)
                row_total += row_len[row][k];
        }

        if (cumulative + row_total > n) {
            target_row = row;
            offset = n - cumulative;
            break;
        }
        cumulative += row_total;
        if (row >= MAX_ROWS - 1)   /* safety net */
            break;
    }

    /* Locate the column k within the target row. */
    idx_t k = 0;
    while (k <= target_row) {
        if (offset < row_len[target_row][k]) break;
        offset -= row_len[target_row][k];
        ++k;
    }

    /* Fetch the digit.  */
    return get_digit(target_row, k, offset, row_len);
}
