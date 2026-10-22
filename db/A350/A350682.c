/*
 * A350682  Möbius values of triangular numbers under divisibility relation.
 * https://oeis.org/A350682
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  sign
 * Formula:   a(n) = μ(T(1), T(n)) where T(k)=k*(k+1)/2 is the k‑th triangular
 *            number and μ is the Möbius function of the poset of triangular
 *            numbers ordered by divisibility
 * Range:     i8 n <= 10000, i16 n <= 10000, i32 n <= 10000, i64 n <= 10000,
 *            i128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

// Computes the Möbius value by dynamic programming:
//
/*  μ(1) = 1
    for n > 1:
        μ(n) = - ∑_{m < n, T(m) | T(n)} μ(m)   */

/*  Only uses integer arithmetic; triangular numbers are kept
    as 64‑bit unsigned, which is sufficient for n ≤ 10000. */

#include <stdlib.h>
#include <stdint.h>

/* The build provides the typedefs:
 *  seq_t – signed integer type (8/16/32/64 bits or arbitrary precision)
 *  idx_t – unsigned integer type (uint64_t)
 */

static idx_t current_max_n = 0;          /* largest n whose values are computed */
static uint64_t *tri = NULL;             /* triangular numbers T(n), 1‑based   */
static seq_t   *mu  = NULL;             /* Möbius values μ(n), 1‑based       */

/* Ensure that values up to n are computed and stored */
static void ensure_up_to(idx_t n) {
    if (n <= current_max_n) return;      /* already computed */

    /* Allocate larger arrays */
    idx_t new_size = n + 1;              /* indices 0..n, we ignore 0 */
    tri = (uint64_t *)realloc(tri, new_size * sizeof(uint64_t));
    mu  = (seq_t   *)realloc(mu,  new_size * sizeof(seq_t));

    /* If this is the first call, initialise for n=1 */
    if (current_max_n == 0) {
        tri[1] = 1ULL;
        mu[1]  = 1; /* μ(1) = 1 */
        current_max_n = 1;
    }

    /* Compute values for k = current_max_n+1 .. n */
    for (idx_t k = current_max_n + 1; k <= n; ++k) {
        /* T(k) = T(k-1) + k */
        tri[k] = tri[k-1] + (uint64_t)k;

        seq_t sum = 0;
        uint64_t Tk = tri[k];
        for (idx_t m = 1; m < k; ++m) {
            if (Tk % tri[m] == 0) {
                sum += mu[m];
            }
        }
        mu[k] = (seq_t)(-sum);            /* μ(k) = -∑ μ(m) */
    }

    current_max_n = n;
}

/* Public function required by the build */
seq_t a(idx_t n) {
    if (n == 0) return 0; /* safety, though n ≥ 1 per specification */
    ensure_up_to(n);
    return mu[n];
}
