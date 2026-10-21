/*
 * A258961  Number of (n+2) X (3+2) 0..1 arrays with no 3 X 3 subblock
 *          diagonal sum 0 and no antidiagonal sum 3 and no row sum 0 or 3 and
 *          no column sum 0 or 3.
 * https://oeis.org/A258961
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 3*a(n-2) - 2*a(n-4) for n > 10 Initial values: a(1)=214,
 *            a(2)=274, a(3)=192, a(4)=286, a(5)=288, a(6)=412, a(7)=456,
 *            a(8)=670, a(9)=816, a(10)=1194
 * Range:     u8 n <= 1, u16 n <= 21, u32 n <= 53, u64 n <= 117, u128 n <= 210,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

static const seq_t init[11] = {
    0,        /* dummy for 0 index */
    214, 274, 192, 286, 288, 412, 456, 670, 816, 1194
};

seq_t a(idx_t n)
{
    if (n <= 10)
        return init[(size_t)n];

    /* Allocate an array to hold the sequence up to n. */
    seq_t *seq = (seq_t *)malloc((n + 1) * sizeof(seq_t));
    if (!seq)
        return 0;                  /* allocation failure – return 0 */

    /* initialise the first 10 values */
    for (idx_t i = 1; i <= 10; ++i)
        seq[i] = init[i];

    /* compute subsequent values using the recurrence */
    for (idx_t i = 11; i <= n; ++i) {
        seq_t t1 = seq[i-2];
        t1 = t1 + t1 + t1;          /* 3 * seq[i-2] */
        seq_t t2 = seq[i-4];
        t2 = t2 + t2;               /* 2 * seq[i-4] */
        seq[i] = t1 - t2;
    }

    seq_t result = seq[n];
    free(seq);
    return result;
}
