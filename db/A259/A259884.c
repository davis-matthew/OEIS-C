/*
 * A259884  Expansion of phi(x) * f(-x^3)^3 / f(-x) in powers of x where
 *          phi(), f() are Ramanujan theta functions.
 * https://oeis.org/A259884
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) is the Euler transform of the periodic sequence *
 *            [3,-2,0,0,3,-5,3,0,0,-2,3,-3] (period 12). The recurrence used
 *            is * a(0)=1 and * a(n) = (1/n) * Σ_{k=1}^n ( Σ_{d|k} d*b(d) ) *
 *            a(n-k), * where b(d) is the periodic sequence above. All
 *            arithmetic is done * with signed 64‑bit integers; the fina...
 * Range:     u8 n <= 1000, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdlib.h>
#include <stdint.h>

seq_t a(idx_t n) {
    /* Cache previous results so that repeated calls are fast. */
    static seq_t *cache = NULL;
    static idx_t cache_size = 0;

    if (n < cache_size)                /* already computed */
        return cache[n];

    /* Compute all terms up to n (inclusive). */
    idx_t N = n;

    /* Temporary signed arrays for the recurrence. */
    int64_t *R = (int64_t *)malloc((N + 1) * sizeof(int64_t));
    int64_t *A = (int64_t *)malloc((N + 1) * sizeof(int64_t));
    if (!R || !A) {                     /* allocation failed */
        free(R); free(A);
        return 0;
    }

    /* Periodic b(k) values. */
    const int period[12] = {3, -2, 0, 0, 3, -5, 3, 0, 0, -2, 3, -3};

    /* Initialize divisor sums R(k) = Σ_{d|k} d*b(d). */
    for (idx_t i = 0; i <= N; ++i) R[i] = 0;
    for (idx_t d = 1; d <= N; ++d) {
        int b = period[(d - 1) % 12];
        int64_t val = (int64_t)d * (int64_t)b;   /* signed product */
        for (idx_t m = d; m <= N; m += d) {
            R[m] += val;
        }
    }

    /* Compute a(0)=1 and subsequent terms using the recurrence. */
    A[0] = 1;
    for (idx_t cur = 1; cur <= N; ++cur) {
        int64_t sum = 0;
        for (idx_t k = 1; k <= cur; ++k) {
            sum += R[k] * A[cur - k];
        }
        A[cur] = sum / (int64_t)cur;   /* division is exact */
    }

    /* Resize cache if necessary and store the computed values. */
    if (N + 1 > cache_size) {
        seq_t *new_cache = (seq_t *)realloc(cache, (N + 1) * sizeof(seq_t));
        if (!new_cache) {         /* allocation failed – clean up */
            free(R); free(A);
            return 0;
        }
        cache = new_cache;
        cache_size = N + 1;
    }
    for (idx_t i = 0; i <= N; ++i) {
        cache[i] = (seq_t)A[i];   /* cast signed to seq_t (unsigned). */
    }

    /* Free temporary buffers. */
    free(R);
    free(A);

    return cache[n];
}
