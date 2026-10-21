/*
 * A245310  G.f. A(x) satisfies: [x^(n+1)] A(x)^n = n*([x^(n-1)] A(x)^n) for
 *          n>=1.
 * https://oeis.org/A245310
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 8, u16 n <= 12, u32 n <= 19, u64 n <= 32, u128 n <= 54,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Recurrence for A245310:
   a(0) = a(1) = a(2) = 1
   for n >= 3:
       a(n) = (n-2)*a(n-2) + Σ_{k=3}^{n-1} (n-k-1) * a(k) * a(n-k) */
#include <stdlib.h>

/* seq_t and idx_t are defined by the build system. */
static seq_t *a_vals = NULL;   /* cache of computed a(n) values   */
static size_t  cap    = 0;     /* allocated capacity of a_vals   */
static idx_t   last   = 2;     /* largest index computed so far  */

seq_t a(idx_t n)
{
    /* initialise cache with base values */
    if (a_vals == NULL) {
        size_t init = 3;                  /* indices 0 .. 2 inclusive */
        a_vals = (seq_t*)malloc(init * sizeof(seq_t));
        a_vals[0] = a_vals[1] = a_vals[2] = (seq_t)1;
        cap = init;
        last = 2;
    }

    /* return already known value */
    if (n <= last)
        return a_vals[(size_t)n];

    /* grow cache if necessary */
    if ((size_t)(n + 1) > cap) {
        size_t new_cap = (size_t)(n + 1);
        seq_t *new_buf = (seq_t*)malloc(new_cap * sizeof(seq_t));
        /* copy existing values */
        for (idx_t i = 0; i <= last; ++i)
            new_buf[i] = a_vals[i];
        free(a_vals);
        a_vals = new_buf;
        cap = new_cap;
    }

    /* compute missing values up to n */
    for (idx_t i = last + 1; i <= n; ++i) {
        seq_t sum = 0;
        for (idx_t k = 3; k <= i - 1; ++k) {
            /* term = a[k] * a[i-k] * (i - k - 1) */
            seq_t prod = a_vals[k] * a_vals[i - k];
            seq_t factor = (seq_t)(i - k - 1);
            sum += prod * factor;
        }
        a_vals[i] = ((seq_t)(i - 2)) * a_vals[i - 2] + sum;
    }

    last = n;
    return a_vals[(size_t)n];
}
