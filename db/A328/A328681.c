/*
 * A328681  a(n) = Sum_{k=1..n} binomial(n,k) * tau(k) * tau(n - k + 1), where
 *          tau = A000005.
 * https://oeis.org/A328681
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Sum_{k=1..n} binomial(n,k) * tau(k) * tau(n - k + 1)
 * Range:     u8 n <= 5, u16 n <= 12, u32 n <= 28, u64 n <= 32, u128 n <= 32,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

static unsigned int *tau_cache = NULL;   /* divisor counts up to current cache size */
static idx_t tau_cache_size = 0;         /* maximum n cached */

/* Recompute tau_cache up to size n (if needed) */
static void ensure_tau(idx_t n)
{
    if (n <= tau_cache_size)
        return;

    /* allocate new cache */
    unsigned int *new_cache = (unsigned int *)malloc((n + 1) * sizeof(unsigned int));
    if (!new_cache) {
        /* Allocation failed – abort execution */
        abort();
    }

    /* zero-initialize and compute divisor counts */
    for (idx_t i = 1; i <= n; ++i)
        new_cache[i] = 0;

    for (idx_t i = 1; i <= n; ++i) {
        for (idx_t j = i; j <= n; j += i)
            new_cache[j] += 1;
    }

    /* replace old cache */
    if (tau_cache)
        free(tau_cache);
    tau_cache = new_cache;
    tau_cache_size = n;
}

seq_t a(idx_t n)
{
    if (n == 0)   /* behavior below offset is irrelevant */
        return 0;

    ensure_tau(n);

    seq_t sum = 0;
    seq_t binom = 1;   /* C(n,0) */

    for (idx_t k = 1; k <= n; ++k) {
        binom = (binom * (seq_t)(n - k + 1)) / (seq_t)k;                 /* C(n,k) */
        seq_t term = binom * (seq_t)tau_cache[k] * (seq_t)tau_cache[n - k + 1];
        sum += term;
    }

    return sum;
}
