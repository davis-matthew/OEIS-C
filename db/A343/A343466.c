/*
 * A343466  a(n) = -(1/n) * Sum_{d|n} phi(n/d) * (-4)^d.
 * https://oeis.org/A343466
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  sign
 * Formula:   a(n) = -(1/n) * Σ_{d|n} φ(n/d) * (-4)^d
 * Range:     i8 n <= 3, i16 n <= 7, i32 n <= 15, i64 n <= 27, i128 n <= 27,
 *            big any n
 * Helpers:   phi, pow_uint
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>
#include "../utils/phi.h"
#include "../utils/pow_uint.h"

static seq_t pow_minus_four(idx_t exp)
{
    seq_t pow4 = pow_uint((uint64_t)4, exp);   /* compute 4^exp */
    return (exp & 1) ? -pow4 : pow4;          /* apply sign (-4)^exp */
}

seq_t a(idx_t n)
{
    if (n == 0) return 0;                    /* not used, guard against div‑by‑zero */

    /* static cache of φ values up to the largest n seen so far */
    static seq_t *phi_cache = NULL;
    static size_t cache_size = 0;             /* number of slots allocated */

    if (cache_size <= n) {
        size_t new_size = n + 1;               /* indices 0 … n (0 unused) */
        seq_t *tmp = (seq_t *)realloc(phi_cache, new_size * sizeof(seq_t));
        if (!tmp) return 0;                  /* allocation failure fallback */
        for (size_t i = cache_size; i <= n; ++i)
            tmp[i] = phi((idx_t)i);           /* compute φ(i) */
        phi_cache = tmp;
        cache_size = new_size;
    }

    seq_t sum = 0;
    for (idx_t d = 1; d * d <= n; ++d) {
        if (n % d == 0) {
            idx_t e = n / d;

            /* term for divisor d */
            seq_t phi_e = phi_cache[e];
            seq_t pow_d = pow_minus_four(d);
            sum += phi_e * pow_d;

            /* term for divisor e, if distinct from d */
            if (d != e) {
                seq_t phi_d = phi_cache[d];
                seq_t pow_e = pow_minus_four(e);
                sum += phi_d * pow_e;
            }
        }
    }

    return -(sum) / (seq_t)n;                /* final formula */
}
