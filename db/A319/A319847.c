/*
 * A319847  Irregular triangle read by rows in which row n lists the odd
 *          divisors of n in increasing order together with the even divisors
 *          of n in increasing order.
 * https://oeis.org/A319847
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, tabf
 * Formula:   not stated
 * Range:     u8 n <= 95, u16 n <= 95, u32 n <= 95, u64 n <= 95, u128 n <= 95,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* a(n) defined by reading a triangle where row r lists the odd divisors of r in increasing order,
   followed by the even divisors of r in increasing order. The linear sequence a(1), a(2), … is obtained
   by reading these rows sequentially. */
#include <stdint.h>
#include <stddef.h>

static inline size_t divisor_count_u64(idx_t n) {
    size_t cnt = 0;
    for (idx_t d = 1; d <= n / d; ++d) {
        if (n % d == 0) {
            if (d == n / d)
                ++cnt;
            else
                cnt += 2;
        }
    }
    return cnt;
}

seq_t a(idx_t n) {
    if (n == 0) return 0; /* offset is 1; guard against zero */
    idx_t cumulative = 0;    /* number of terms in all previous rows */
    idx_t row = 1;           /* current row number */

    /* Find the row that contains the n-th term */
    while (1) {
        size_t rowDivCount = divisor_count_u64(row);
        if (cumulative + rowDivCount >= n) break;
        cumulative += rowDivCount;
        ++row;
    }

    /* Position within the found row (0‑based) */
    idx_t pos = n - cumulative - 1;

    /* Search for the pos‑th divisor, odd divisors first */
    idx_t odd_found = 0;
    for (idx_t d = 1; d <= row; ++d) {
        if (row % d == 0 && (d & 1)) {          /* odd divisor */
            if (odd_found == pos) return (seq_t)d;
            ++odd_found;
        }
    }

    /* If not found among odd divisors, search among even divisors */
    idx_t even_pos = pos - odd_found;
    idx_t even_found = 0;
    for (idx_t d = 1; d <= row; ++d) {
        if (row % d == 0 && (!(d & 1))) {      /* even divisor */
            if (even_found == even_pos) return (seq_t)d;
            ++even_found;
        }
    }

    return 0; /* should never reach here */
}
