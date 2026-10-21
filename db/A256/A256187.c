/*
 * A256187  First differences of Per Nørgård's "infinity sequence" A004718.
 * https://oeis.org/A256187
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign
 * Formula:   a(n) = A004718(n+1) - A004718(n)
 * Range:     i8 n <= 10000, i16 n <= 10000, i32 n <= 10000, i64 n <= 10000,
 *            i128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

static seq_t *b = NULL;       /* cache for A004718 values */
static idx_t b_cap = 0;       /* size of the allocated cache */
static idx_t b_done = 0;      /* largest index computed (inclusive) */

/* Ensure the cache can hold indices up to n (inclusive). */
static void ensure_capacity(idx_t n)
{
    if ((idx_t)(n + 1) > b_cap) {
        idx_t new_cap = n + 1;
        seq_t *tmp = (seq_t *)realloc(b, (size_t)new_cap * sizeof(seq_t));
        if (!tmp) abort();
        b = tmp;
        b_cap = new_cap;
    }
}

/* Compute A004718(k) for all k from b_done+1 up to n (inclusive). */
static void compute_b_upto(idx_t n)
{
    ensure_capacity(n);
    for (idx_t i = b_done + 1; i <= n; ++i) {
        if ((i & 1) == 0)           /* even: b(2i) = -b(i) */
            b[i] = -(b[i / 2]);
        else                        /* odd:  b(2i+1) = b(i) + 1 */
            b[i] = b[(i - 1) / 2] + (seq_t)1;
    }
    b_done = n;
}

/* Public sequence function */
seq_t a(idx_t n)
{
    /* Initialise cache on first call */
    if (b == NULL) {
        b_cap = 2;               /* indices 0 and 1 */
        b_done = 1;
        b = (seq_t *)malloc(b_cap * sizeof(seq_t));
        if (!b) abort();
        b[0] = (seq_t)0;
        b[1] = (seq_t)1;         /* A004718(0)=0, A004718(1)=1 */
    }

    /* Need values up to index n+1 to compute the difference */
    if (n + 1 > b_done)
        compute_b_upto(n + 1);

    return b[n + 1] - b[n];
}
