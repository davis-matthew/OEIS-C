/*
 * A307179  Numbers k such that k = i*j = 6*i + j, where i and j are integers.
 * https://oeis.org/A307179
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  sign, fini, full
 * Formula:   not stated
 * Range:     i8 n <= 8, i16 n <= 8, i32 n <= 8, i64 n <= 8, i128 n <= 8,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A307179: solutions k = i * j of the equation i * j = 6 * i + j
   (i and j integers).  The sequence consists of the eight values
   –25, –8, –3, 0, 24, 27, 32, 49 arranged in increasing order. */

#include <stdlib.h>

static int cmp_seq_t(const void *p, const void *q)
{
    const seq_t a = *(const seq_t *)p;
    const seq_t b = *(const seq_t *)q;
    return (a < b) ? -1 : (a > b) ? 1 : 0;
}

static seq_t values[8];
static int initialized = 0;

static void init(void)
{
    if (initialized) return;
    const int divs[4] = {1, 2, 3, 6};
    size_t idx = 0;
    for (int k = 0; k < 4; ++k) {
        int d = divs[k];
        seq_t i = (seq_t)(1 + d);
        seq_t j = (seq_t)6 + (seq_t)6 / d;
        values[idx++] = i * j;
        int nd = -d;
        seq_t i2 = (seq_t)(1 + nd);
        seq_t j2 = (seq_t)6 + (seq_t)6 / nd;
        values[idx++] = i2 * j2;
    }
    qsort(values, 8, sizeof(seq_t), cmp_seq_t);
    initialized = 1;
}

seq_t a(idx_t n)
{
    init();
    if (n < 1 || n > 8) return (seq_t)0;
    return values[(size_t)n - 1];
}
