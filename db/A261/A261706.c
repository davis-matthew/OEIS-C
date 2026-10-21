/*
 * A261706  Number of (n+2) X (3+2) 0..1 arrays with each 3 X 3 subblock
 *          having clockwise perimeter pattern 00000001 00000101 or 00000111.
 * https://oeis.org/A261706
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 4*a(n-3) +3*a(n-6) +4*a(n-7) -2*a(n-9) +4*a(n-11)
 *            -8*a(n-13) +10*a(n-14) (empirical recurrence valid for n > 20)
 *            Initial values: a(1)=172, a(2)=270, a(3)=468, a(4)=662,
 *            a(5)=1168, a(6)=2372, a(7)=3700, a(8)=6158, a(9)=11812,
 *            a(10)=19350, a(11)=31112, a(12)=58428, a(13)=98160,
 *            a(14)=157340,...
 * Range:     u8 n <= 1, u16 n <= 12, u32 n <= 32, u64 n <= 74, u128 n <= 156,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n) {
    if (n == 0)
        return (seq_t)0;

    /* Persistent cache of computed values (0-indexed: values[0] = a(1)) */
    static seq_t *values = NULL;
    static idx_t values_len = 0;            /* number of entries stored */
    static idx_t values_cap = 0;            /* allocated capacity */

    /* Initial table for n = 1 .. 20 */
    static const seq_t init[20] = {
        (seq_t)172,  (seq_t)270,  (seq_t)468,  (seq_t)662,
        (seq_t)1168, (seq_t)2372, (seq_t)3700, (seq_t)6158,
        (seq_t)11812, (seq_t)19350, (seq_t)31112, (seq_t)58428,
        (seq_t)98160, (seq_t)157340, (seq_t)290888, (seq_t)493946,
        (seq_t)796540, (seq_t)1451708, (seq_t)2482588, (seq_t)4029944
    };

    /* Allocate cache on first call */
    if (!values) {
        values_cap = 20;
        values_len = 20;
        values = (seq_t *)malloc(values_cap * sizeof(seq_t));
        if (!values) abort(); /* allocation failed */
        for (idx_t i = 0; i < 20; ++i)
            values[i] = init[i];
    }

    /* If the requested n is already cached, return it */
    if (n <= values_len)
        return values[n - 1];

    /* Extend the cache to hold up to n */
    if (n > values_cap) {
        idx_t new_cap = n;
        seq_t *new_ptr = (seq_t *)realloc(values, new_cap * sizeof(seq_t));
        if (!new_ptr) abort();
        values = new_ptr;
        values_cap = new_cap;
    }

    /* Compute new terms using the empirical recurrence */
    for (idx_t i = values_len; i < n; ++i) {
        /* recurrence applies for i >= 20 (n >= 21) */
        seq_t pos = values[i - 3] * 4
                  + values[i - 6] * 3
                  + values[i - 7] * 4
                  + values[i - 11] * 4
                  + values[i - 14] * 10;

        seq_t neg = values[i - 9] * 2
                  + values[i - 13] * 8;

        values[i] = pos - neg;   /* pos >= neg by definition of the sequence */
    }

    values_len = n;
    return values[n - 1];
}
