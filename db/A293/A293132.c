/*
 * A293132  G.f.: 2*q * Product_{n>=1} (1 + q^(2*n))/((1 + q^n)*(1 +
 *          q^(2*n-1))*(1 + q^(4*n))) in powers of q.
 * https://oeis.org/A293132
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  sign
 * Formula:   not stated
 * Range:     i8 n <= 10, i16 n <= 34, i32 n <= 117, i64 n <= 418,
 *            i128 n <= 1547, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* 
   Computes OEIS A293132: 
   G.f. = 2*q * Product_{n>=1} (1 + q^(2*n))/((1 + q^n)*(1 + q^(2*n-1))*(1 + q^(4*n)))
   The sequence is produced for n >= 1.
*/

#include <stddef.h>
#include <stdint.h>

static void precompute(idx_t max_n, seq_t *f)
{
    /* Size of arrays */
    size_t N = (size_t)max_n;
    size_t i, j;

    /* Work arrays */
    static seq_t A[10001];
    static seq_t B[10001];
    static seq_t tmp[10001];
    static seq_t E[10001];

    /* Initialize A and B to 1 */
    A[0] = 1;
    for (i = 1; i <= N; ++i) A[i] = 0;
    B[0] = 1;
    for (i = 1; i <= N; ++i) B[i] = 0;

    /* Compute A = product_{k>=1} (1+q^{4k})^{-1}  (division by each factor once) */
    for (i = 4; i <= N; i += 4) {
        /* Copy current A into tmp */
        for (j = 0; j <= N; ++j) tmp[j] = A[j];
        /* Apply division by (1+q^i) using new[j] = old[j] - new[j-i] */
        for (j = i; j <= N; ++j) {
            tmp[j] -= tmp[j - i];
        }
        /* Move result back to A */
        for (j = 0; j <= N; ++j) A[j] = tmp[j];
    }

    /* Compute B = product_{m odd>=1} (1+q^m)^{-2}
       (division by each factor twice) */
    for (i = 1; i <= N; i += 2) {
        /* First division */
        for (j = 0; j <= N; ++j) tmp[j] = B[j];
        for (j = i; j <= N; ++j) {
            tmp[j] -= tmp[j - i];
        }
        for (j = 0; j <= N; ++j) B[j] = tmp[j];

        /* Second division */
        for (j = 0; j <= N; ++j) tmp[j] = B[j];
        for (j = i; j <= N; ++j) {
            tmp[j] -= tmp[j - i];
        }
        for (j = 0; j <= N; ++j) B[j] = tmp[j];
    }

    /* Convolution: E = A * B  (up to degree N) */
    for (i = 0; i <= N; ++i) E[i] = 0;
    for (i = 0; i <= N; ++i) {
        if (A[i] == 0) continue;
        for (j = 0; j + i <= N; ++j) {
            if (B[j] == 0) continue;
            E[i + j] += A[i] * B[j];
        }
    }

    /* Final sequence: a(n) = 2 * E[n-1] for n >= 1 */
    for (i = 1; i <= N; ++i) {
        f[i] = (seq_t)2 * E[i - 1];
    }
}

seq_t a(idx_t n)
{
    /* The maximum n expected from the tests (≈10000) */
    const idx_t MAX_N = 10000;

    static int initialized = 0;
    static seq_t cache[10001];

    if (!initialized) {
        precompute(MAX_N, cache);
        initialized = 1;
    }

    /* If a requested n exceeds the precomputed limit,
       recompute with a larger size (rare case). */
    if (n > MAX_N) {
        seq_t *new_cache = (seq_t*)malloc((size_t)(n + 1) * sizeof(seq_t));
        if (!new_cache) return (seq_t)0; /* out of memory */
        precompute(n, new_cache);
        /* Free old cache if it was allocated beyond 10001 */
        if (MAX_N != 10000) free(cache);
        cache[n] = new_cache[n];
        return new_cache[n];
    }

    return cache[(size_t)n];
}
