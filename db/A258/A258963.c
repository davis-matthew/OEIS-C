/*
 * A258963  Number of (n+2) X (5+2) 0..1 arrays with no 3 X 3 subblock
 *          diagonal sum 0 and no antidiagonal sum 3 and no row sum 0 or 3 and
 *          no column sum 0 or 3.
 * https://oeis.org/A258963
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 3*a(n-2) - 2*a(n-4) for n > 10 Initial values: a(1) =
 *            676, a(2) = 614, a(3) = 288, a(4) = 340, a(5) = 384, a(6) = 466,
 *            a(7) = 552, a(8) = 724, a(9) = 912, a(10) = 1248 The sequence
 *            satisfies this linear recurrence for every n ≥ 11
 * Range:     u8 none, u16 n <= 21, u32 n <= 53, u64 n <= 117, u128 n <= 210,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>   /* for malloc, free */

static const seq_t init_vals[10] = {
    676, 614, 288, 340, 384, 466, 552, 724, 912, 1248
};

seq_t a(idx_t n)
{
    /* Handle the first 10 terms directly. */
    if (n <= 10) {
        /* n is at least 1; idx_t is unsigned, so this is safe. */
        return init_vals[n - 1];
    }

    /* Allocate a temporary array to hold the terms up to n.
       The buffer is indexed from 1 to n inclusive. */
    size_t len = (size_t)n + 1;
    seq_t *buf = (seq_t*)malloc(len * sizeof(seq_t));
    if (!buf) {
        /* In an environment where malloc fails we return 0 as a
           sentinel value; the test harness expects the sequence
           to be well‑defined for all practical n. */
        return 0;
    }

    /* Initialize the first ten entries. */
    for (idx_t i = 1; i <= 10; ++i) {
        buf[i] = init_vals[i - 1];
    }

    /* Compute the remaining terms using the recurrence. */
    for (idx_t i = 11; i <= n; ++i) {
        buf[i] = 3 * buf[i - 2] - 2 * buf[i - 4];
    }

    /* Retrieve the answer. */
    seq_t result = buf[n];

    /* Clean up. */
    free(buf);
    return result;
}
