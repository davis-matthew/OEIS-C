/*
 * A294463  E.g.f.: Product_{k>0} (1-k*x^k)^(1/k).
 * https://oeis.org/A294463
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign
 * Formula:   a(0) = 1 a(n) = -(n-1)! * Σ_{k=1..n} A055225(k) * a(n-k) /
 *            (n-k)! (n>0) A055225(k) = Σ_{d|k} d^{k/d} Sequence A294463
 *            Implementation uses memoisation to answer queries in any order
 * Range:     i8 n <= 4, i16 n <= 7, i32 n <= 11, i64 n <= 19, i128 n <= 32,
 *            big any n
 * Helpers:   pow_int, seq_realloc
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>
#include "../utils/pow_int.h"
#include "../utils/seq_realloc.h"

static seq_t *a_vals = NULL;   /* a(n) values, a_vals[0] = 1 */
static seq_t *S      = NULL;   /* A055225(k) values, S[0] unused */
static idx_t  capacity = 0;    /* size of allocated arrays (capacity-1 = highest index stored) */
static idx_t  computed_max = 0;/* largest n for which a_vals[n] is computed */

/* Ensure arrays can hold indices 0..n */
static void ensure_capacity(idx_t n)
{
    if (n + 1 <= capacity)
        return; /* already sufficient */

    seq_t *new_a = seq_realloc(a_vals, capacity, n + 1);
    seq_t *new_S = seq_realloc(S,       capacity, n + 1);

    a_vals = new_a;
    S      = new_S;

    /* Initialize new slots to zero */
    for (idx_t i = capacity; i <= n; ++i) {
        a_vals[i] = (seq_t)0;
        S[i]      = (seq_t)0;
    }

    if (capacity == 0) {
        a_vals[0] = (seq_t)1;   /* a(0) = 1 */
    }

    capacity = n + 1;
}

/* Compute A055225(k) for k in [start..end], assuming start>=1 */
static void compute_S_part(idx_t start, idx_t end)
{
    for (idx_t i = start; i <= end; ++i) {
        if (i == 0) continue;
        seq_t sum = (seq_t)0;
        for (idx_t d = 1; d * d <= i; ++d) {
            if (i % d == 0) {
                /* term for divisor d */
                seq_t term1 = pow_int((seq_t)d, (idx_t)(i / d));
                sum += term1;
                /* term for complementary divisor d2 = i/d if distinct */
                if (d != i / d) {
                    idx_t d2 = i / d;
                    seq_t term2 = pow_int((seq_t)d2, (idx_t)(i / d2));
                    sum += term2;
                }
            }
        }
        S[i] = sum;
    }
}

/* Compute a(n) for n in [start..end]   (start >= 1) */
static void compute_terms(idx_t start, idx_t end)
{
    for (idx_t n = start; n <= end; ++n) {
        seq_t sum = (seq_t)0;
        seq_t F   = (seq_t)1;   /* F = (n-1)!/(n-k)! for current k, starts as 1 for k=1 */
        for (idx_t k = 1; k <= n; ++k) {
            seq_t term = S[k] * a_vals[n - k] * F;
            sum += term;
            /* Prepare F for next k: multiply by (n-k) */
            if (k < n)   /* avoid multiplying by zero when k==n */
                F = F * (seq_t)(n - k);
        }
        a_vals[n] = -sum;
    }
}

/* Compute all needed values up to n (inclusive) */
static void compute_up_to(idx_t n)
{
    ensure_capacity(n);

    if (computed_max < n) {
        /* Compute S for new indices */
        compute_S_part(computed_max + 1, n);
        /* Compute a for new indices */
        compute_terms(computed_max + 1, n);
        computed_max = n;
    }
}

seq_t a(idx_t n)
{
    if (n == 0)
        return (seq_t)1; /* a(0) = 1 */
    compute_up_to(n);
    return a_vals[n];
}
