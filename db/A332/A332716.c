/*
 * A332716  Number of compositions of n^2 where each part is less than or
 *          equal to n.
 * https://oeis.org/A332716
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = number of compositions of n^2 with all parts ≤ n
 *            (equivalently, number of compositions of n^2 with parts 1..n).
 *            This function implements a linear‐recurrence DP with a sliding
 *            window: for n fixed, let f[0]=1 and for k≥1, f[k] = f[k-1] +
 *            f[k-2] + … + f[k-n] (terms with negative index are 0)....
 * Range:     u8 n <= 3, u16 n <= 4, u32 n <= 5, u64 n <= 8, u128 n <= 11,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdlib.h>

static seq_t *buf = NULL;               /* ring buffer for last n values   */
static size_t buf_cap = 0;              /* current capacity of buf         */

/* Ensure the ring buffer can hold at least n elements, allocating
   and zeroing the buffer if a reallocation occurs. */
static void ensure_buf(idx_t n)
{
    if ((size_t)n > buf_cap) {
        free(buf);
        buf = (seq_t *)calloc((size_t)n, sizeof(seq_t));
        buf_cap = n;
    }
}

/* Computes a(n) as defined above. */
seq_t a(idx_t n)
{
    if (n == 0) {                /* a(0) = 1 by definition           */
        return (seq_t)1;
    }

    /* Prepare the ring buffer for this n. */
    ensure_buf(n);
    for (idx_t i = 0; i < n; ++i) {
        buf[i] = (seq_t)0;      /* reset all entries to 0           */
    }
    buf[0] = (seq_t)1;          /* f[0] = 1                          */
    seq_t sum = (seq_t)1;       /* sum of current window: initially f[0] */
    idx_t idx = 1 % n;          /* next position to overwrite       */

    seq_t cur = (seq_t)0;        /* will hold f[k]       */
    idx_t N = n * n;             /* target index k = n^2 (fits 64‑bit for n≤~4.3e9) */

    for (idx_t k = 1; k <= N; ++k) {
        cur = sum;                     /* f[k] = sum of previous n values */

        /* Update the running sum: add new cur, subtract the
           value being overwritten at buf[idx]. */
        sum = sum + cur - buf[idx];

        buf[idx] = cur;                 /* insert new value */
        idx = (idx + 1) % n;            /* advance ring position */
    }
    return cur;                        /* f[n^2] */
}
