/*
 * A258747  Expansion of chi(-x) * f(x^3) * f(-x^6) in powers of x where
 *          chi(), f() are Ramanujan theta functions.
 * https://oeis.org/A258747
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign
 * Formula:   a(0)=1; for n>0 n*a(n) = Σ_{k=1..n} ( Σ_{d|k} d*b(d) ) * a(n-k),
 *            where b(d) is the period‑12 sequence b(d) =
 *            [-1,0,0,0,-1,-3,-1,0,0,0,-1,-2] (indices start at 1)
 * Range:     i8 n <= 61, i16 n <= 4645, i32 n <= 10000, i64 n <= 10000,
 *            i128 n <= 10000, big any n
 * Helpers:   ensure_large
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>
#include <stddef.h>   /* for size_t */
#include <string.h>   /* for memset in C mode */
#include "../utils/ensure_large.h"

seq_t a(idx_t n)
{
    static seq_t *aseq = NULL;
    static size_t a_cap = 0;
    static idx_t max_a = 0;
    static bool initialized = false;

    /* ensure storage for indices 0..n */
    ensure_large(n + 1, &aseq, &a_cap, sizeof(seq_t));
    if (!initialized) {
        aseq[0] = 1;
        initialized = true;
        max_a = 0;
    }

    if (n <= max_a) return aseq[n];

    /* divisor‑sum array T[k] = Σ_{d|k} d*b(d) */
    seq_t *T = (seq_t *)malloc((n + 1) * sizeof(seq_t));
    if (!T) exit(1);
    for (idx_t i = 0; i <= n; ++i) T[i] = 0;

    const int period[12] = { -1, 0, 0, 0, -1, -3, -1, 0, 0, 0, -1, -2 };

    for (idx_t d = 1; d <= n; ++d) {
        int b = period[(d - 1) % 12];
        if (b == 0) continue;
        seq_t add = (seq_t)d * (seq_t)b;
        for (idx_t m = d; m <= n; m += d)
            T[m] += add;
    }

    for (idx_t i = max_a + 1; i <= n; ++i) {
        seq_t sum = 0;
        for (idx_t k = 1; k <= i; ++k)
            sum += T[k] * aseq[i - k];
        aseq[i] = sum / (seq_t)i;
    }

    free(T);
    max_a = n;
    return aseq[n];
}
