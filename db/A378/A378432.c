/*
 * A378432  Dirichlet inverse of A296075, where A296075 is the sum of
 *          deficiencies of divisors of n.
 * https://oeis.org/A378432
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  sign
 * Formula:   not stated
 * Range:     i8 n <= 101, i16 n <= 9970, i32 n <= 9970, i64 n <= 9970,
 *            i128 n <= 9970, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A378432: Dirichlet inverse of A296075.
   Let B(n) = ∑_{d|n} (2*d – σ(d)), the sum of deficiencies of all divisors of n.
   Then a(1) = 1, and for n > 1
        a(n) = –∑_{d|n, d<n} B(n/d) * a(d). */

#include <stddef.h>
#include <stdint.h>
#include <stdlib.h>

/* Internal static state */
static seq_t *A_cache = NULL;
static seq_t *B_cache = NULL;
static idx_t precomp_n = 0;

/* Forward declaration */
static void compute_up_to(idx_t N);

/* Public sequence function */
seq_t a(idx_t n) {
    if (n == 0) return 0;                /* offset is 1, n>=1 always */
    if (n > precomp_n) compute_up_to(n);
    return A_cache[n];
}

/* Helper: compute σ(n) and sum_{d|n} σ(d) up to N,
   then B[n] = 2*σ[n] – sumσ[n], and finally a[n] via recurrence. */
static void compute_up_to(idx_t N) {
    idx_t size = N + 1;
    /* Allocate auxiliary arrays */
    seq_t *sigma   = (seq_t*)calloc((size_t)size, sizeof(seq_t));
    seq_t *sum_sigma = (seq_t*)calloc((size_t)size, sizeof(seq_t));
    if (!sigma || !sum_sigma) exit(1);

    /* Compute σ(n): sum of divisors */
    for (idx_t i = 1; i <= N; ++i) {
        for (idx_t j = i; j <= N; j += i) {
            sigma[j] += (seq_t)i;
        }
    }

    /* Compute sum_{d|n} σ(d) */
    for (idx_t i = 1; i <= N; ++i) {
        for (idx_t j = i; j <= N; j += i) {
            sum_sigma[j] += sigma[i];
        }
    }

    /* Allocate new caches */
    seq_t *newA = (seq_t*)malloc((size_t)size * sizeof(seq_t));
    seq_t *newB = (seq_t*)malloc((size_t)size * sizeof(seq_t));
    if (!newA || !newB) exit(1);

    /* Initialize for n = 1 */
    newA[1] = 1;
    newB[1] = 2 * sigma[1] - sum_sigma[1];    /* =1 */

    /* Compute B[n] for n >= 2 */
    for (idx_t n = 2; n <= N; ++n) {
        newB[n] = 2 * sigma[n] - sum_sigma[n];
    }

    /* Compute a[n] via recurrence */
    for (idx_t n = 2; n <= N; ++n) {
        seq_t s = 0;
        /* iterate over proper divisors d of n */
        for (idx_t d = 1; d * d <= n; ++d) {
            if (n % d == 0) {
                idx_t e = n / d;
                if (d < n) {
                    s += newB[e] * newA[d];
                }
                if (e != d && e < n) {
                    s += newB[d] * newA[e];
                }
            }
        }
        newA[n] = -s;
    }

    /* Free old caches */
    free(A_cache);
    free(B_cache);
    free(sigma);
    free(sum_sigma);

    /* Commit new caches */
    A_cache = newA;
    B_cache = newB;
    precomp_n = N;
}
