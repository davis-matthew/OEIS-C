/*
 * A291316  Expansion of x/(1-x) + x^4*(1-x)/(1-x^3) +
 *          x^7*(1-x)*(1-x^3)/(1-x^5) + ... in powers of x.
 * https://oeis.org/A291316
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  sign
 * Formula:   a(n) = coefficient of x^n in x/(1-x) + x^4*(1-x)/(1-x^3) +
 *            x^7*(1-x)*(1-x^3)/(1-x^5) +
 * Range:     i8 n <= 79, i16 n <= 79, i32 n <= 79, i64 n <= 79, i128 n <= 79,
 *            big any n
 * Validated: partial, pass, 79 terms through n = 79 (2026-11-03)
 * Helpers:   mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include "../utils/mem.h"

seq_t a(idx_t n)
{
    if (n == 0)
        return 0;                       /* offset is 1 */

    size_t N = (size_t)n + 1;            /* array size */
    seq_t *coeff = mem_new(seq_t, N);
    seq_t *f     = mem_new(seq_t, N);
    if (!coeff || !f) {
        mem_free(coeff);
        mem_free(f);
        return 0;                       /* allocation failed */
    }

    f[0] = 1;                            /* N_0 = 1 */

    for (idx_t k = 0; ; ++k) {
        idx_t s0 = 1 + 3 * k;             /* starting exponent of T_k */
        if (s0 > n)
            break;                       /* no more terms needed */
        idx_t step = 2 * k + 1;           /* step of the geometric series */

        /* Add contributions of T_k to coeff[] */
        for (idx_t p = 0; p <= n; ++p) {
            seq_t val = f[p];
            if (val == 0)
                continue;
            idx_t start = s0 + p;
            if (start > n)
                continue;
            for (idx_t pos = start; pos <= n; pos += step) {
                coeff[pos] += val;
            }
        }

        /* Update f to N_{k+1} by multiplying by (1 - x^step) */
        if (step <= n) {
            for (idx_t d = n; d >= step; --d) {
                f[d] = f[d] - f[d - step];
            }
        }
    }

    seq_t result = coeff[n];
    mem_free(coeff);
    mem_free(f);
    return result;
}
