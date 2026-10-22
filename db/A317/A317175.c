/*
 * A317175  Square array T(n, k) read by antidiagonals upwards, n > 0 and k >
 *          0: T(n, k) is the least m > 0 such that m * n contains k as a
 *          substring in its decimal representation.
 * https://oeis.org/A317175
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base, tabl
 * Formula:   not stated
 * Range:     u8 n <= 82, u16 n <= 82, u32 n <= 82, u64 n <= 82, u128 n <= 82,
 *            big any n
 * Helpers:   seq_to_str
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A317175:
   Enumerate the square array T(n,k) by anti‑diagonals upwards.
   T(n,k) is the smallest positive integer m such that the decimal
   representation of m*n contains the decimal representation of k as a
   substring. */

#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <math.h>
#include "../utils/seq_to_str.h"

static seq_t compute_T(idx_t row, idx_t col)
{
    /* Convert the column value to a decimal string once */
    seq_t col_seq = (seq_t)col;
    char col_buf[64];
    size_t col_len;
    seq_to_str(col_seq, col_buf, &col_len);
    col_buf[col_len] = '\0';

    /* Upper bound given by the OEIS entry: T(n,k) <= 10*k */
    seq_t limit = col_seq * 10;

    for (seq_t m = 1; ; ++m) {
        seq_t prod = (seq_t)row * m;
        char prod_buf[512];
        size_t prod_len;
        seq_to_str(prod, prod_buf, &prod_len);
        prod_buf[prod_len] = '\0';

        if (strstr(prod_buf, col_buf) != NULL)
            return m;

        if (m > limit)   /* safety net, should never be reached */
            return limit;
    }
}

static uint64_t find_diagonal(idx_t n)
{
    /* Compute smallest s such that (s-1)*s/2 >= n.
       Solve s = ceil((1+sqrt(1+8*n))/2). */
    long double d = sqrt((long double)(1.0L + 8.0L * (long double)n));
    uint64_t s = (uint64_t)((1.0L + d) / 2.0L);
    if ((unsigned __int128)(s - 1) * s / 2 < n)
        ++s;
    return s;
}

seq_t a(idx_t n)
{
    /* Map the index n to array position (row, col) along anti‑diagonals */
    uint64_t s = find_diagonal(n);                /* sum n+ k */
    uint64_t prev_total = (s - 2) * (s - 1) / 2;   /* total before this diag */
    uint64_t offset   = n - prev_total;           /* 1‑based position in the diag */

    uint64_t row = s - offset;   /* row number (n) */
    uint64_t col = offset;       /* column number (k) */

    return compute_T(row, col);
}
