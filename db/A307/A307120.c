/*
 * A307120  a(1) = 3, for n>1, a(n) = A000005(n-1) + A000005(n) +
 *          A000005(n+1).
 * https://oeis.org/A307120
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = tau(n-1) + tau(n) + tau(n+1) for n>1; a(1) = 3 where
 *            tau(k) = number of positive divisors of k (A000005)
 * Range:     u8 n <= 79, u16 n <= 79, u32 n <= 79, u64 n <= 79, u128 n <= 79,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

static seq_t *tau_cache = NULL;
static idx_t tau_size = 0;      /* allocated size (number of elements) */
static idx_t max_cached = 0;    /* maximum n whose tau value is valid */

/* compute divisor counts for all numbers 1..m inclusive, store in tau_cache,
   reallocate as necessary. */
static void ensure_tau_cache(idx_t m) {
    if (m <= max_cached)
        return;
    /* create new array of size m+1 (index 0 unused) */
    seq_t *new_cache = (seq_t *)calloc(m + 1, sizeof(seq_t));
    if (!new_cache) {
        /* fallback: abort, as we cannot recover */
        exit(EXIT_FAILURE);
    }
    /* simple sieve for divisor counts */
    for (idx_t i = 1; i <= m; ++i) {
        for (idx_t j = i; j <= m; j += i) {
            new_cache[j] = new_cache[j] + 1;
        }
    }
    /* free old cache */
    free(tau_cache);
    tau_cache = new_cache;
    tau_size = m + 1;
    max_cached = m;
}

seq_t a(idx_t n) {
    if (n == 0) {
        /* Undefined for n=0, but return 0 to avoid UB */
        return (seq_t)0;
    }
    if (n == 1)
        return (seq_t)3;

    /* ensure tau counts up to n+1 are available */
    ensure_tau_cache(n + 1);
    seq_t a_val =
        tau_cache[n - 1] + tau_cache[n] + tau_cache[n + 1];
    return a_val;
}
