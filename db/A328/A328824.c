/*
 * A328824  Numerators of A113405(-n) (see the comment for details).
 * https://oeis.org/A328824
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign, easy, frac
 * Formula:   a(n) = a(n-1) - 8*a(n-3) + 8*a(n-4) Initial values: a(0)=0,
 *            a(1)=1, a(2)=1, a(3)=1
 * Range:     i8 n <= 9, i16 n <= 18, i32 n <= 33, i64 n <= 66, i128 n <= 129,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* This uses a memoised iterative recurrence; it works for any
   seq_t type (fixed‑width or arbitrary‑precision) and handles
   calls in any order. */
#include <stdlib.h>

/* seq_t and idx_t are defined by the build system. */
seq_t a(idx_t n)
{
    /* static cache of computed values */
    static seq_t *buf = NULL;   /* array of values */
    static idx_t cap = 0;       /* allocated capacity (in terms of indices) */
    static idx_t last = 0;      /* highest index computed so far */

    /* initialise the first four values on the first call */
    if (buf == NULL) {
        cap = 4;
        buf = (seq_t*)malloc(cap * sizeof(seq_t));
        if (!buf) return 0;      /* allocation failed – return 0 */
        buf[0] = 0;
        buf[1] = 1;
        buf[2] = 1;
        buf[3] = 1;
        last = 3;
    }

    /* expand the buffer capacity if needed */
    if (n > last) {
        while (cap <= n) {
            idx_t new_cap = cap ? (cap << 1) : 16;
            if (new_cap <= n) new_cap = n + 1;
            buf = (seq_t*)realloc(buf, new_cap * sizeof(seq_t));
            if (!buf) return 0;  /* allocation failed – return 0 */
            cap = new_cap;
        }

        /* compute missing values */
        for (idx_t i = last + 1; i <= n; ++i) {
            buf[i] = buf[i-1]
                     - (seq_t)8 * buf[i-3]
                     + (seq_t)8 * buf[i-4];
        }
        last = n;
    }

    return buf[n];
}
