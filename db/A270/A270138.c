/*
 * A270138  Continued fraction expansion of the constant
 *          6/A270121(1)+Sum_{n>=2}1/A270121(n).
 * https://oeis.org/A270138
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, cofr
 * Formula:   not stated
 * Range:     u8 n <= 6, u16 n <= 6, u32 n <= 8, u64 n <= 10, u128 n <= 10,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A270138
 *
 *  a(0) = 0
 *  a(1) = 1
 *  a(2) = 6
 *  For n ≥ 1  :
 *      a(2*n+1) = n * A270124(n-1)
 *      a(2*n+2) = A270121(n)
 *
 *  A270121 is defined by
 *      x(1)  = 7
 *      x(2)  = 112
 *      x(n+1) = x(n)^2 * (1 + n*x(n)) / x(n-1)    for n ≥ 2
 *
 *  A270124 is defined by
 *      y(0) = 2
 *      y(n) = x(n+1) / x(n)    for n ≥ 1
 *
 *  The function below lazily builds the sequence A270121 and uses it to
 *  compute A270138 on demand.  It works for any seq_t / idx_t that
 *  satisfies the operations listed in the problem statement.
 */

#include <stdlib.h>

static seq_t *x = NULL;          /* A270121 values, 1‑indexed */
static idx_t  x_cap = 0;          /* largest index currently filled */

/* Ensure that A270121 values are known up to index m (m ≥ 1). */
static void ensure_x_upto(idx_t m)
{
    if (m <= x_cap) return;                   /* nothing to do */

    /* Resize array to hold indices 0 .. m+1 (0 unused). */
    size_t new_cap = (size_t)m + 2;
    seq_t *tmp = (seq_t *)realloc(x, new_cap * sizeof(seq_t));
    if (!tmp && new_cap > 0) abort();         /* out of memory */
    x = tmp;

    /* Initialise the base values if they have not yet been set. */
    if (x_cap < 1) x[1] = (seq_t)7;
    if (x_cap < 2 && m >= 2) x[2] = (seq_t)112;

    /* Compute remaining values.  For any i ≥ 3 we use
     *    x[i] = x[i-1]^2 * (1 + (i-1)*x[i-1]) / x[i-2].
     */
    idx_t start = (x_cap < 2 ? (idx_t)3 : x_cap + 1);
    for (idx_t i = start; i <= m; ++i) {
        seq_t prev = x[i - 1];
        seq_t prev_sq = prev * prev;                 /* x[i-1]^2          */
        seq_t inner = (seq_t)(i - 1) * prev + (seq_t)1; /* 1 + (i-1)*x[i-1] */
        seq_t num = prev_sq * inner;                  /* numerator        */
        seq_t den = x[i - 2];
        x[i] = num / den;                            /* integer division */
    }

    x_cap = m;  /* we now have values up to index m */
}

/* Public function: return the n‑th term of A270138. */
seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0;
    if (n == 1) return (seq_t)1;
    if (n == 2) return (seq_t)6;

    if ((n & 1U) == 1) {                 /* odd index: n = 2*m+1 */
        idx_t m = (n - 1) >> 1;          /* m ≥ 1 */
        if (m == 1) return (seq_t)2;     /* A270124(0) = 2 */

        /* A270124(m-1) = x[m] / x[m-1] */
        ensure_x_upto(m);
        seq_t y = x[m] / x[m - 1];
        return (seq_t)m * y;             /* n * A270124(n-1) */
    } else {                             /* even index: n = 2*m+2 */
        idx_t m = (n - 2) >> 1;          /* m ≥ 1 */
        ensure_x_upto(m);
        return x[m];                     /* A270121(m)      */
    }
}
