/*
 * A319109  Expansion of Product_{k>=1} 1/(1 + x^k)^(k-1).
 * https://oeis.org/A319109
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign
 * Formula:   not stated
 * Range:     i8 n <= 13, i16 n <= 32, i32 n <= 50, i64 n <= 50, i128 n <= 50,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* OEIS A319109: Expansion of Product_{k>=1} 1/(1 + x^k)^(k-1)
 *
 * The coefficients a(n) satisfy the recurrence
 *   a(0) = 1
 *   a(n) = (1/n) * Σ_{k=1..n}  C_k * a(n-k)    (n≥1)
 *
 * where
 *   C_k = Σ_{d | k} d(d-1) * (-1)^(k/d)
 *
 * This follows from the logarithm of the generating function.
 *
 * The implementation pre‑computes C_k up to the requested n and then
 * evaluates the recurrence in O(n²) time, which is fast enough for
 * n up to about 10 000 and works for all supported seq_t sizes.
 */

#include <stdlib.h>

/* Global cache of coefficients and sequence values */
static idx_t   maxN = 0;   /* largest index currently computed   */
static seq_t *C  = NULL;   /* C_k coefficients (size = maxN+1)  */
static seq_t *A  = NULL;   /* a(n) values (size = maxN+1)      */

/* Recompute all data up to N (inclusive).  The previous cache is freed. */
static void recompute_until(idx_t N)
{
    /* allocate new arrays */
    seq_t *Cnew = (seq_t*)malloc((N + 1) * sizeof(seq_t));
    seq_t *Anew = (seq_t*)malloc((N + 1) * sizeof(seq_t));
    if (!Cnew || !Anew) exit(1);

    /* zero initialize Cnew */
    for (idx_t i = 0; i <= N; ++i) Cnew[i] = 0;

    /* compute C_k for k = 1 .. N */
    for (idx_t d = 1; d <= N; ++d) {
        seq_t val = (seq_t)d * (seq_t)(d - 1);      /* d(d-1) */
        for (idx_t k = d, m = 1; k <= N; k += d, ++m) {
            seq_t sign = (m & 1) ? (seq_t)-1 : (seq_t)1; /* (-1)^m */
            Cnew[k] += val * sign;
        }
    }

    /* compute a(n) using the recurrence */
    Anew[0] = 1;
    for (idx_t n = 1; n <= N; ++n) {
        seq_t sum = 0;
        for (idx_t k = 1; k <= n; ++k) {
            sum += Cnew[k] * Anew[n - k];
        }
        Anew[n] = sum / (seq_t)n;   /* division is exact */
    }

    /* free old cache and store new pointers */
    free(C);
    free(A);
    C   = Cnew;
    A   = Anew;
    maxN = N;
}

/* Public function required by the build system */
seq_t a(idx_t n)
{
    if (A == NULL || n > maxN) recompute_until(n);
    return A[n];
}
