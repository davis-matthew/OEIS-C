/*
 * A331182  Number of values of k, 1 <= k <= n, with A083254(k) = A083254(n),
 *          where A083254(n) = 2*phi(n) - n.
 * https://oeis.org/A331182
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 505, u16 n <= 9842, u32 n <= 9842, u64 n <= 9842,
 *            u128 n <= 9842, big any n
 * Helpers:   build_primes_upto
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Returns a(n) = the number of k (1 ≤ k ≤ n) for which
   A083254(k) = A083254(n), where A083254(m) = 2*phi(m) - m. */
/* The function computes φ up to n and then counts matches. */

#include <stdlib.h>
#include "../utils/build_primes_upto.h"

/* Static cache for Euler's totient values. */
static seq_t *phi_cache = NULL;
static idx_t phi_cache_max = 0;    /* maximum n for which phi_cache is valid */

/* Ensure φ values are available up to n.  Recomputes all values
   when n increases.  This is O(n log log n) which is fine for n ≤ 10k. */
static void ensure_phi_up_to(idx_t n)
{
    if (n <= phi_cache_max)
        return;

    /* Allocate (n+1) entries (indices 0..n). */
    size_t new_size = (size_t)(n + 1);
    seq_t *tmp = (seq_t *)realloc(phi_cache, new_size * sizeof(seq_t));
    if (!tmp) {
        /* Allocation failed: leave the cache empty. */
        free(phi_cache);
        phi_cache = NULL;
        phi_cache_max = 0;
        return;
    }
    phi_cache = tmp;

    /* Initialize phi[i] = i for all entries. */
    for (idx_t i = 0; i <= n; ++i)
        phi_cache[i] = (seq_t)i;

    /* Obtain all primes up to n. */
    uint64_t *primes = NULL;
    size_t prime_cnt = 0;
    build_primes_upto(n, &primes, &prime_cnt);

    /* Apply the sieve to compute φ. */
    for (size_t pi = 0; pi < prime_cnt; ++pi) {
        idx_t p = (idx_t)primes[pi];
        for (idx_t m = p; m <= n; m += p)
            phi_cache[m] -= phi_cache[m] / (seq_t)p;
    }

    free(primes);
    phi_cache_max = n;
}

seq_t a(idx_t n)
{
    if (n < 1)
        return 0;   /* outside the offset – arbitrary */

    /* Make sure φ values up to n are available. */
    ensure_phi_up_to(n);
    if (phi_cache == NULL)
        return 0;   /* allocation failed */

    /* Compute target value g(n) = 2*φ(n) - n. */
    seq_t target = phi_cache[n] * 2 - (seq_t)n;

    /* Count all k <= n with g(k) == target. */
    seq_t count = 0;
    for (idx_t k = 1; k <= n; ++k) {
        seq_t val = phi_cache[k] * 2 - (seq_t)k;
        if (val == target)
            ++count;
    }
    return count;
}
