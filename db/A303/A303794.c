/*
 * A303794  Number of nX2 0..1 arrays with every element unequal to 0, 1, 2 or
 *          4 king-move adjacent elements, with upper left element zero.
 * https://oeis.org/A303794
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 2*a(n-1) + 8*a(n-3) - 8*a(n-4) - 8*a(n-5); initial
 *            values: a(1)=2, a(2)=4, a(3)=12, a(4)=24, a(5)=64, a(6)=184
 * Range:     u8 n <= 6, u16 n <= 12, u32 n <= 24, u64 n <= 47, u128 n <= 94,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    /* Use 0‑based indices, buf[0] unused. */
    static seq_t *buf = NULL;
    static idx_t buf_cap = 0;      /* allocated size (size_t) */
    static idx_t last_computed = 0;/* highest index computed so far */

    if (n < 1)
        return 0;

    /* Ensure enough capacity, doubling strategy. */
    if (n + 1 > buf_cap) {
        idx_t new_cap = buf_cap > 0 ? buf_cap << 1 : 64;
        while (new_cap < n + 1)
            new_cap <<= 1;
        seq_t *tmp = (seq_t *)realloc(buf, (size_t)new_cap * sizeof(seq_t));
        if (!tmp) return 0; /* allocation failure, should not happen */
        buf = tmp;
        buf_cap = new_cap;
    }

    /* Initialize base values (1..6). */
    if (last_computed < 6) {
        buf[1] = (seq_t)2;
        buf[2] = (seq_t)4;
        buf[3] = (seq_t)12;
        buf[4] = (seq_t)24;
        buf[5] = (seq_t)64;
        buf[6] = (seq_t)184;
        if (last_computed < 6)
            last_computed = 6;
    }

    /* Compute new terms up to n using the recurrence. */
    for (idx_t i = last_computed + 1; i <= n; ++i) {
        seq_t term = 0;
        term = (seq_t)2 * buf[i - 1];
        term += (seq_t)8 * buf[i - 3];
        term -= (seq_t)8 * buf[i - 4];
        term -= (seq_t)8 * buf[i - 5];
        buf[i] = term;
    }
    last_computed = n;

    return buf[n];
}
