/*
 * A251293  Number of (n+1) X (1+1) 0..3 arrays with every 2 X 2 subblock
 *          summing to 3 6 or 9.
 * https://oeis.org/A251293
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 10*a(n-1) - 21*a(n-2) - 20*a(n-3) with a(1)=84, a(2)=444,
 *            a(3)=2356
 * Range:     u8 n <= 1, u16 n <= 4, u32 n <= 11, u64 n <= 24, u128 n <= 49,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h> /* realloc, abort */

seq_t a(idx_t n)
{
    if (n < 1) {
        return 0;
    }

    static seq_t *buf = NULL;
    static idx_t sz = 0;
    static idx_t last_computed = 0;

    /* enlarge buffer if needed */
    if (sz < n) {
        seq_t *tmp = (seq_t *)realloc(buf, n * sizeof(seq_t));
        if (!tmp) abort();
        buf = tmp;
        sz = n;
    }

    /* initialise first three terms once */
    if (last_computed < 3) {
        if (sz >= 1) buf[0] = (seq_t)84;
        if (sz >= 2) buf[1] = (seq_t)444;
        if (sz >= 3) buf[2] = (seq_t)2356;
        last_computed = 3;
    }

    /* compute missing values up to n */
    for (idx_t i = last_computed; i < n; ++i) {
        buf[i] = (seq_t)10 * buf[i-1] - (seq_t)21 * buf[i-2] - (seq_t)20 * buf[i-3];
    }
    last_computed = n;

    return buf[n-1];
}
