/*
 * A325211  a(1) = 1; a(n) = Sum_{d|n, d<n} tau(n/d) * a(d), where tau =
 *          A000005.
 * https://oeis.org/A325211
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Σ_{d|n, d<n} τ(n/d) * a(d), a(1)=1
 * Range:     u8 n <= 23, u16 n <= 383, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Recurrence variant: a(1) = 1; for n>1, a(n) = Σ_{d|n, d<n} τ(n/d) * a(d) */

#include <stdlib.h>

/* Static caches */
static seq_t *a_cache = NULL;     /* a values indexed by n (0 unused) */
static seq_t *tau_cache = NULL;  /* τ values for indices up to current size-1 */
static idx_t cache_size = 0;     /* allocated array size (including index 0) */
static idx_t computed_upto = 0;  /* largest n for which a_cache[n] is known */

/* Allocate or enlarge caches up to the requested maximum n */
/* All indices are 0..n inclusive (size = n+1) */
static void ensure_cache(idx_t n)
{
    if (n + 1 <= cache_size)
        return;  /* already large enough */

    /* new size: indices 0 .. n inclusive */
    idx_t new_size = n + 1;

    /* allocate new arrays */
    seq_t *new_a   = (seq_t *)malloc((size_t)new_size * sizeof(seq_t));
    seq_t *new_tau = (seq_t *)malloc((size_t)new_size * sizeof(seq_t));
    if (!new_a || !new_tau) {
        /* allocation failure – abort for safety */
        abort();
    }

    /* zero‑initialize new arrays */
    for (idx_t i = 0; i < new_size; ++i) {
        new_a[i]   = (seq_t)0;
        new_tau[i] = (seq_t)0;
    }

    /* copy old data if present */
    if (a_cache) {
        for (idx_t i = 0; i < cache_size; ++i) {
            new_a[i] = a_cache[i];
        }
        free(a_cache);
        free(tau_cache);
    }

    a_cache   = new_a;
    tau_cache = new_tau;
    cache_size = new_size;

    /* compute divisor counts τ(k) for all k ≤ n */
    for (idx_t i = 1; i < new_size; ++i) {
        for (idx_t j = i; j < new_size; j += i) {
            tau_cache[j] = tau_cache[j] + (seq_t)1;
        }
    }
}

/* Compute a(n) for all n up to “target” (inclusive) */
static void compute_up_to(idx_t target)
{
    if (target <= computed_upto)
        return;  /* already computed */

    ensure_cache(target);

    /* base case */
    if (computed_upto < 1) {
        a_cache[1] = (seq_t)1;
        computed_upto = 1;
    }

    /* compute successive values */
    for (idx_t m = computed_upto + 1; m <= target; ++m) {
        seq_t sum = (seq_t)0;

        /* integer sqrt of m (fast enough for small n) */
        idx_t root = 0;
        while ((root + 1) * (root + 1) <= m)
            ++root;

        for (idx_t d = 1; d <= root; ++d) {
            if (m % d == 0) {
                idx_t e = m / d;
                if (d < m) {
                    sum += tau_cache[e] * a_cache[d];
                }
                if (e != d && e < m) {
                    sum += tau_cache[d] * a_cache[e];
                }
            }
        }
        a_cache[m] = sum;
    }

    computed_upto = target;
}

/* Public sequence function */
seq_t a(idx_t n)
{
    if (n == 0)  /* undefined for n<1, return 0 provisionally */
        return (seq_t)0;
    compute_up_to(n);
    return a_cache[n];
}
