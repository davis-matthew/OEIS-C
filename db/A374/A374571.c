/*
 * A374571  Expansion of g.f. A(x) satisfying A(x) = A(x^2) - x*A(x^2)^2.
 * https://oeis.org/A374571
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign
 * Formula:   not stated
 * Range:     i8 n <= 36, i16 n <= 104, i32 n <= 368, i64 n <= 1308,
 *            i128 n <= 4868, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A374571: a(0)=1;
   For n>0:
      if n even:          a(n)=a(n/2);
      if n odd :          a(n) = - Σ_{k=0..(n-1)/2} a(k)*a((n-1)/2−k)
   This recurrence follows from A(x) = A(x²) – x*A(x²)².  */
#include <stdlib.h>

seq_t a(idx_t n)
{
    /* Cached coefficients:  buf[0..used-1] are computed.
       buf_capacity is the actual allocated size.  */
    static seq_t *buf = NULL;
    static idx_t buf_capacity = 0;
    static idx_t used = 0;          /* number of entries that are valid  */

    /* Ensure the buffer can hold at least n+1 entries.  */
    if (n + 1 > buf_capacity) {
        idx_t new_cap = buf_capacity > 0 ? buf_capacity : 1;
        while (new_cap < n + 1) new_cap <<= 1;   /* grow so that new_cap >= n+1 */

        seq_t *tmp = (seq_t *)realloc(buf, new_cap * sizeof(seq_t));
        if (!tmp) return 0;                       /* allocation failed */
        buf = tmp;
        buf_capacity = new_cap;
    }

    /* If the buffer is empty, initialise a(0)=1.  */
    if (used == 0) {
        buf[0] = 1;
        used = 1;
    }

    /* Compute any missing entries up to n.  */
    if (n + 1 > used) {
        for (idx_t i = used; i <= n; ++i) {
            if ((i & 1ULL) == 0) {                /* even index */
                if (i == 0) {                     /* a(0) is already set */
                    /* nothing to do */
                } else {
                    buf[i] = buf[i / 2];
                }
            } else {                              /* odd index */
                idx_t m = (i - 1) / 2;
                seq_t sum = 0;
                for (idx_t j = 0; j <= m; ++j) {
                    sum += buf[j] * buf[m - j];
                }
                buf[i] = -sum;
            }
        }
        used = n + 1;
    }

    return buf[n];
}
