/*
 * A261320  Expansion of (phi(q^3) / phi(q))^2 in powers of q where phi() is a
 *          Ramanujan theta function.
 * https://oeis.org/A261320
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign
 * Formula:   not stated
 * Range:     i8 n <= 5, i16 n <= 16, i32 n <= 53, i64 n <= 187, i128 n <= 685,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A261320: a(n) = coefficient of q^n in (phi(q^3)/phi(q))^2
   where phi(q) = Σ_{k∈ℤ} q^{k^2}.  
   The algorithm computes φ(q), φ(q^3), inverts φ(q) to obtain 1/φ(q),
   multiplies by φ(q^3), and finally squares the resulting series
   to produce the terms of the sequence.  The implementation uses only
   integer arithmetic and memoises the sequence up to the largest index
   requested.  The routine works for any seq_t (fixed‑size or arbitrary
   precision) and any idx_t.  */
#include <stdlib.h>

static idx_t max_precomputed = 0;
static seq_t *coeff = NULL;

static void compute_all(idx_t N) {
    size_t sz = (size_t)(N + 1);

    if (coeff) free(coeff);
    coeff = (seq_t*)calloc(sz, sizeof(seq_t));

    seq_t *phi  = (seq_t*)calloc(sz, sizeof(seq_t));
    seq_t *phi3 = (seq_t*)calloc(sz, sizeof(seq_t));
    seq_t *inv  = (seq_t*)calloc(sz, sizeof(seq_t));
    seq_t *T    = (seq_t*)calloc(sz, sizeof(seq_t));

    /* phi(q): 1 + 2 Σ_{k≥1} q^{k^2} */
    phi[0] = 1;
    for (idx_t k = 1; k * k <= N; ++k) {
        idx_t s = k * k;
        if (s <= N) phi[s] += (seq_t)2;
    }

    /* phi(q^3): 1 + 2 Σ_{k≥1} q^{3k^2} */
    phi3[0] = 1;
    for (idx_t k = 1; 3 * k * k <= N; ++k) {
        idx_t t = 3 * k * k;
        if (t <= N) phi3[t] += (seq_t)2;
    }

    /* Invert phi: inv[0] = 1, inv[n] = -Σ_{k≥1, k^2≤n} phi[k^2] * inv[n-k^2] */
    inv[0] = 1;
    for (idx_t m = 1; m <= N; ++m) {
        seq_t sum = 0;
        for (idx_t k = 1; k * k <= m; ++k) {
            seq_t v = phi[k * k];
            if (v) sum += v * inv[m - k * k];
        }
        inv[m] = -sum;
    }

    /* T = phi(q^3) * (1/phi(q)) */
    for (idx_t m = 0; m <= N; ++m) {
        seq_t t = inv[m];                 /* phi3[0] contributes 1 * inv[m] */
        for (idx_t k = 1; 3 * k * k <= m; ++k)
            t += (seq_t)2 * inv[m - 3 * k * k];
        T[m] = t;
    }

    /* a[n] = (T * T)[n] = Σ_{k=0}^n T[k] * T[n-k] */
    for (idx_t m = 0; m <= N; ++m) {
        seq_t sum = 0;
        for (idx_t k = 0; k <= m; ++k)
            sum += T[k] * T[m - k];
        coeff[m] = sum;
    }

    free(phi);
    free(phi3);
    free(inv);
    free(T);
    max_precomputed = N;
}

seq_t a(idx_t n) {
    if (coeff == NULL || n > max_precomputed) compute_all(n);
    return coeff[n];
}
