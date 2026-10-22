/*
 * A330073  Irregular triangle read as rows in which row n is the result of
 *          iterating the operation f(n) = n/5 if n == 0 (mod 5), otherwise
 *          f(n) = 5*(floor(n/5) + n + 1), terminating at the first occurrence
 *          of 1.
 * https://oeis.org/A330073
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy, tabf
 * Formula:   not stated
 * Range:     u8 n <= 34, u16 n <= 68, u32 n <= 68, u64 n <= 68, u128 n <= 68,
 *            big any n
 * Helpers:   f_next
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*   A330073: irregular triangle read left-to-right
    a(n) is the n-th element of the triangle formed by iterating
    c(m) = { m/5            if m ≡ 0 (mod 5)
          5*(⌊m/5⌋ + m + 1) otherwise
        } until the first appearance of 1.
    Rows are numbered from 1 with T(n,0)=n.  The sequence a(n) is the
    concatenation of the rows.                                                     */

#include <stddef.h>
#include <stdlib.h>
#include <stdint.h>
#include "../utils/f_next.h"

/* forward */
static idx_t row_length(idx_t n);

/* length of the row beginning with n  (number of terms until 1 inclusive) */
static idx_t row_length(idx_t n) {
    seq_t v = (seq_t)n;
    idx_t l = 1;
    while (v != 1) {
        v = f_next(v);
        ++l;
    }
    return l;
}

/* a(n) definition */
seq_t a(idx_t N) {
    /* static cache of cumulative lengths */
    static idx_t *cum = NULL;
    static size_t  cum_cap = 0;
    static idx_t  cum_len = 0;   /* number of rows cached */

    /* expand cache until N is covered */
    while (cum_len == 0 || cum[cum_len - 1] < N) {
        idx_t r = cum_len + 1;               /* next row index */
        idx_t L = row_length(r);             /* length of this row   */
        idx_t newcum = (cum_len == 0 ? 0 : cum[cum_len - 1]) + L;
        if (cum_len == cum_cap) {
            size_t new_cap = cum_cap == 0 ? 64 : cum_cap << 1;
            idx_t *tmp = (idx_t *)realloc(cum, new_cap * sizeof(idx_t));
            if (!tmp) { /* out of memory – abort with zero */
                abort();
            }
            cum = tmp;
            cum_cap = new_cap;
        }
        cum[cum_len] = newcum;
        ++cum_len;
    }

    /* binary search for the row containing N */
    idx_t lo = 0, hi = cum_len - 1;
    while (lo < hi) {
        idx_t mid = lo + (hi - lo) / 2;
        if (cum[mid] >= N)
            hi = mid;
        else
            lo = mid + 1;
    }
    idx_t row = lo + 1;                         /* row numbers start at 1 */
    idx_t prev_cum = (lo == 0) ? 0 : cum[lo - 1];
    idx_t pos = N - prev_cum;                   /* 1‑based position in the row */
    idx_t k = pos - 1;                          /* 0‑based index */

    /* compute T(row,k) */
    seq_t val = (seq_t)row;
    for (idx_t i = 0; i < k; ++i)
        val = f_next(val);

    return val;
}
