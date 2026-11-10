/*
 * A378217  Dirichlet inverse of A330575.
 * https://oeis.org/A378217
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  sign
 * Formula:   a(1) = 1, a(n) = -Sum_{d|n, d<n} A330575(n/d) * a(d), where
 *            A330575(1)=1, A330575(n) = n + Sum_{d|n, d<n} A330575(d)
 * Range:     i8 n <= 77, i16 n <= 18689, i32 n <= 65537, i64 n <= 65537,
 *            i128 n <= 65537, big any n
 * Validated: standard, pass, 93 terms through n = 7901 (2026-11-10)
 * Helpers:   mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>
#include "../utils/mem.h"

static seq_t *A378217_cache = NULL;
static idx_t A378217_max = 0;

seq_t a(idx_t n)
{
    if (n == 0) return 0;
    if (n > A378217_max)
    {
        idx_t cap = n;
        if (cap < 2 * A378217_max) cap = 2 * A378217_max;
        if (cap < 64) cap = 64;

        seq_t *f = mem_new(seq_t, (size_t)cap + 1);
        seq_t *acc = mem_new(seq_t, (size_t)cap + 1);
        seq_t *b = mem_new(seq_t, (size_t)cap + 1);
        if (!f || !acc || !b)
        {
            if (f) mem_free(f);
            if (acc) mem_free(acc);
            if (b) mem_free(b);
            return 0;
        }

        /* f = A330575 via sieve */
        for (idx_t i = 1; i <= cap; ++i) f[i] = (seq_t)i;
        f[1] = 1;
        for (idx_t i = 1; i <= cap; ++i)
            for (idx_t m = 2 * i; m <= cap; m += i)
                f[m] += f[i];

        /* Dirichlet inverse */
        for (idx_t d = 1; d <= cap; ++d)
        {
            if (d == 1) b[1] = 1;
            else b[d] = -acc[d];
            for (idx_t m = 2 * d; m <= cap; m += d)
                acc[m] += f[m / d] * b[d];
        }

        mem_free(f);
        mem_free(acc);
        if (A378217_cache) mem_free(A378217_cache);
        A378217_cache = b;
        A378217_max = cap;
    }
    return A378217_cache[n];
}
