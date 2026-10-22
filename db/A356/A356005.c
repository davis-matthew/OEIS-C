/*
 * A356005  Number of integers k such that k*tau(k) <= n.
 * https://oeis.org/A356005
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = number of k with k*tau(k) <= n (tau(k) is the number of
 *            positive divisors of k)
 * Range:     u8 n <= 79, u16 n <= 79, u32 n <= 79, u64 n <= 79, u128 n <= 79,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>

/* seq_t and idx_t are provided by the build environment */

static idx_t *tau_cache = NULL;        /* divisor counts for 1..N */
static idx_t tau_cache_size = 0;       /* maximum index currently cached */

/* Recompute divisor counts up to 'limit' (inclusive) */
static void
ensure_tau(idx_t limit)
{
    if (tau_cache_size >= limit)
        return;                        /* already have enough */

    /* allocate new array of idx_t and compute divisor counts */
    idx_t *new_cache = (idx_t *)calloc((size_t)(limit + 1), sizeof(idx_t));
    if (!new_cache)
        abort();                       /* out of memory */

    for (idx_t i = 1; i <= limit; ++i)
        for (idx_t j = i; j <= limit; j += i)
            ++new_cache[j];

    free(tau_cache);
    tau_cache = new_cache;
    tau_cache_size = limit;
}

/* Sequence function: counts k with k * tau(k) <= n */
seq_t a(idx_t n)
{
    if (n == 0)
        return 0;                      /* undefined, but safe */

    /* build divisor table up to n */
    ensure_tau(n);

    seq_t count = 0;
    for (idx_t k = 1; k <= n; ++k)
    {
        /* use 128‑bit arithmetic to avoid overflow on 64‑bit builds */
        unsigned __int128 prod = (unsigned __int128)k *
                                 (unsigned __int128)tau_cache[k];
        if (prod <= (unsigned __int128)n)
            ++count;
    }
    return count;
}
