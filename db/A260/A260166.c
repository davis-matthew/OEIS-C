/*
 * A260166  Expansion of phi(x^2) * f(-x^3)^3 / chi(-x)^2 in powers of x where
 *          phi(), chi(), f() are Ramanujan theta functions.
 * https://oeis.org/A260166
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 127, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* 
 * OEIS A260166: a(n) = expansion of phi(x^2) * f(-x^3)^3 / chi(-x)^2
 * 
 * Computation via the product
 *   ∏_{m≥1} (1 - x^m)^{e_m},
 * where e_m = -2 + 3*[3|m] + 5*[4|m] - 2*[8|m].
 * The coefficients a(n) satisfy
 *   a(0) = 1,
 *   a(n) = (1/n) * Σ_{k=1..n} ( -S_k ) * a(n-k),
 *   with S_k = Σ_{d|k} d * e_d.
 * The code below implements this recurrence exactly for all n ≥ 0.
 */

#include <stdint.h>
#include <stdlib.h>

seq_t a(idx_t n)
{
    /* special case of n = 0 */
    if (n == (idx_t)0) {
        return (seq_t)1;
    }

    /* For local stack usage we rely on dynamic allocation for arrays up to n */
    size_t N = (size_t)n;         /* N fits in size_t for n ≤ 64‑bit */

    /* Allocate space for e[1..N] and S[1..N] as signed 64‑bit integers */
    int64_t *e = (int64_t *)calloc(N + 1, sizeof(int64_t));
    int64_t *S = (int64_t *)calloc(N + 1, sizeof(int64_t));
    if (e == NULL || S == NULL) {
        free(e); free(S);
        return (seq_t)0;           /* allocation failure */
    }

    /* Compute e[d] values. e[0] unused. */
    for (size_t d = 1; d <= N; ++d) {
        int64_t val = -2;
        if (d % 3U == 0U) val += 3;    /* 3|d */
        if (d % 4U == 0U) val += 5;    /* 4|d */
        if (d % 8U == 0U) val -= 2;    /* 8|d */
        e[d] = val;
    }

    /* Compute S[m] = Σ_{d|m} d * e[d] via a divisor sieve */
    for (size_t d = 1; d <= N; ++d) {
        int64_t term = (int64_t)d * e[d];
        for (size_t m = d; m <= N; m += d) {
            S[m] += term;
        }
    }

    /* Allocate space for a[0..N] */
    seq_t *arr = (seq_t *)malloc((N + 1) * sizeof(seq_t));
    if (arr == NULL) {
        free(e); free(S);
        return (seq_t)0;
    }

    arr[0] = (seq_t)1;   /* a(0) = 1 */

    /* Recurrence: a(n) = (1/n) * Σ_{k=1..n} (-S_k) * a(n-k) */
    for (size_t i = 1; i <= N; ++i) {
        int64_t acc = 0;                      /* signed intermediate */
        for (size_t k = 1; k <= i; ++k) {
            acc += -S[k] * (int64_t)arr[i - k];
        }
        /* division is exact; cast to seq_t at the end */
        arr[i] = (seq_t)(acc / (int64_t)i);
    }

    seq_t result = arr[N];

    /* Free temporary storage */
    free(arr);
    free(e);
    free(S);

    return result;
}
