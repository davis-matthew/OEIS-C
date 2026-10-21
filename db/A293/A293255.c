/*
 * A293255  G.f.: Product_{m>0} (1 - x^m + 2!*x^(2*m) - 3!*x^(3*m)).
 * https://oeis.org/A293255
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign
 * Formula:   a(n) = coefficient of x^n in Π_{m>0} (1 - x^m + 2!*x^(2m) -
 *            3!*x^(3m)) This is implemented by successive convolution of the
 *            factors
 * Range:     i8 n <= 17, i16 n <= 71, i32 n <= 238, i64 n <= 500,
 *            i128 n <= 500, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

static seq_t *a_cache = NULL;          /* array of computed coefficients */
static idx_t   cache_size  = 0;        /* number of elements stored (0..size-1) */

/* Return a(n).  Recompute the whole table when a larger n is requested. */
seq_t a(idx_t n)
{
    /* Recompute cache if n is beyond current size */
    if (n >= cache_size) {
        idx_t new_size = n + 1;
        seq_t *new_cache = (seq_t *)calloc(new_size, sizeof(seq_t));
        if (!new_cache) return 0;          /* allocation failed */

        new_cache[0] = 1;                  /* multiplicative identity */

        /* Multiply the generating function by each factor in turn */
        for (idx_t m = 1; m <= n; ++m) {
            idx_t m2 = m + m;              /* 2m */
            idx_t m3 = m2 + m;             /* 3m */
            for (idx_t t = n; t >= m; --t) {
                seq_t val = new_cache[t];
                val -= new_cache[t - m];
                if (t >= m2) val += 2 * new_cache[t - m2];
                if (t >= m3) val -= 6 * new_cache[t - m3];
                new_cache[t] = val;
            }
        }

        /* Replace old cache */
        free(a_cache);
        a_cache    = new_cache;
        cache_size = new_size;
    }

    return a_cache[n];
}
