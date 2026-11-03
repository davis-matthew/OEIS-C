/*
 * A257536  Expansion of phi(-x^4)^2 * f(-x^1, -x^5) in powers of x where
 *          phi(), f() are Ramanujan theta functions.
 * https://oeis.org/A257536
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign
 * Formula:   a(n) is the Euler transform of the period‑24 sequence b(k) = [
 *            -1,0,0,-4,-1,-1,-1,-2,0,0,-1,-5,-1,0,0,-2,-1,-1,-1,-4,0,0,-1,-3
 *            ]. That is, 1 + Σ_{n ≥ 1} a(n) x^n = ∏_{k ≥ 1} (1 -
 *            x^k)^(-b(k)). Equivalently, a(0)=1 and for n ≥ 1 n · a(n) =
 *            Σ_{k=1..n} ( Σ_{d|k} d·b(d) ) · a(n-k)
 * Range:     i8 n <= 15, i16 n <= 678, i32 n <= 2500, i64 n <= 2500,
 *            i128 n <= 2500, big any n
 * Validated: standard, pass, 92 terms through n = 2264 (2026-11-03)
 * Helpers:   mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <string.h>
#include "../utils/mem.h"

static const seq_t period24[24] = {
    -1, 0, 0, -4, -1, -1, -1, -2,
     0, 0, -1, -5, -1, 0, 0, -2,
    -1, -1, -1, -4, 0, 0, -1, -3
};

seq_t a(idx_t n) {
    if (n == 0) return (seq_t)1;

    /* Allocate arrays up to n (inclusive) */
    seq_t *g = mem_new(seq_t, (n + 1));   /* g[k] = Σ_{d|k} d·b(d) */
    seq_t *A = mem_new(seq_t, (n + 1));   /* a[k]  */
    if (!g || !A) {
        /* In case of out‑of‑memory; return 0 as a safe fallback */
        if (g) mem_free(g);
        if (A) mem_free(A);
        return (seq_t)0;
    }
    mem_set(g, 0, (n + 1) * sizeof(seq_t));
    mem_set(A, 0, (n + 1) * sizeof(seq_t));

    /* Pre‑compute g[k] via a divisor sieve */
    for (idx_t d = 1; d <= n; ++d) {
        seq_t b = period24[(d - 1) % 24];
        seq_t prod = (seq_t)d * b;
        for (idx_t m = d; m <= n; m += d) {
            g[m] += prod;
        }
    }

    /* Compute a[0] */
    A[0] = (seq_t)1;

    /* Compute a[1..n] using the recurrence */
    for (idx_t i = 1; i <= n; ++i) {
        seq_t sum = 0;
        for (idx_t k = 1; k <= i; ++k) {
            sum += g[k] * A[i - k];
        }
        A[i] = sum / (seq_t)i;   /* division is exact */
    }

    seq_t result = A[n];
    mem_free(g);
    mem_free(A);
    return result;
}
