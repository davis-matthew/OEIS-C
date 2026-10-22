/*
 * A397275  Irregular triangle read by rows in which row m lists the full
 *          period of triangular numbers modulo m.
 * https://oeis.org/A397275
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy, tabf
 * Formula:   not stated
 * Range:     u8 n <= 9215, u16 n <= 9215, u32 n <= 9215, u64 n <= 9215,
 *            u128 n <= 9215, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence: a(n) is the n-th element of the irregular triangle in which
 * row m lists the full period of triangular numbers modulo m.
 * The i-th term of row m (0‑based) equals ((i*(i+1))/2) mod m. */

#include <stdlib.h>
#include <stddef.h>

seq_t a(idx_t n)
{
    /* static cache of cumulative row lengths:
     * cum[i] = total number of entries up to row (i+1) (1‑based). */
    static idx_t *cum = NULL;      /* cumulative sums */
    static size_t cum_len = 0;     /* number filled */
    static size_t cum_cap = 0;     /* allocated capacity */

    /* initialize cache */
    if (cum_len == 0) {
        cum_cap = 16;
        cum = (idx_t *)malloc(cum_cap * sizeof(idx_t));
        if (!cum) abort();
    }

    /* extend cache until it covers the requested n */
    idx_t sum = (cum_len > 0) ? cum[cum_len - 1] : 0;
    idx_t m = cum_len;           /* current row index (1‑based) */
    while (sum < n) {
        ++m;                     /* next row index */
        idx_t L = (m & 1) ? m : 2 * m;  /* period length of row m */
        sum += L;

        if (cum_len == cum_cap) {
            cum_cap *= 2;
            idx_t *tmp = (idx_t *)realloc(cum, cum_cap * sizeof(idx_t));
            if (!tmp) abort();
            cum = tmp;
        }
        cum[cum_len++] = sum;
    }

    /* compute position inside the identified row m */
    idx_t L = (m & 1) ? m : 2 * m;          /* length of row m */
    idx_t prev_sum = sum - L;               /* entries before row m */
    idx_t k = n - prev_sum - 1;             /* 0‑based column index */

    /* triangular number modulo m */
    idx_t tri = k * (k + 1) / 2;            /* fits comfortably in idx_t */
    seq_t val = (seq_t)(tri % (idx_t)m);
    return val;
}
