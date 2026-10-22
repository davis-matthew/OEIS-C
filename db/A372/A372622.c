/*
 * A372622  a(n) = (1/4) * Sum_{k=1..n} phi(5*k).
 * https://oeis.org/A372622
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = (1/4) * Sum_{k=1..n} phi(5*k)
 * Range:     u8 n <= 13, u16 n <= 57, u32 n <= 57, u64 n <= 57, u128 n <= 57,
 *            big any n
 * Helpers:   build_primes
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdbool.h>
#include "../utils/build_primes.h"

/* static cache for Euler's totient values */
static seq_t *phi_cache = NULL;
static idx_t phi_cache_len = 0;

/* static cache for primes */
static idx_t *primes_cache = NULL;
static size_t primes_cache_cnt = 0;

/* Ensure primes up to 'limit' are available */
static void ensure_primes_up_to(idx_t limit) {
    if (primes_cache && primes_cache_cnt > 0 &&
        (idx_t)primes_cache[primes_cache_cnt - 1] >= limit) {
        return;
    }
    /* free old prime list */
    free(primes_cache);
    primes_cache = NULL;
    primes_cache_cnt = 0;
    /* build new list of primes up to limit */
    size_t cnt = 0;
    build_primes(limit, &primes_cache, &cnt);
    primes_cache_cnt = cnt;
}

/* Build totient array up to N (inclusive) */
static void build_phi_up_to(idx_t N) {
    if (N <= phi_cache_len)
        return;

    /* allocate new buffer */
    seq_t *new_phi = (seq_t *)calloc((size_t)(N + 1), sizeof(seq_t));
    if (!new_phi)
        return;                      /* allocation failed, abort silently */

    for (idx_t i = 0; i <= N; ++i)
        new_phi[i] = (seq_t)i;       /* initial value */

    /* ensure we have prime list */
    ensure_primes_up_to(N);

    /* totient sieve: for each prime subtract its contribution from all multiples */
    for (size_t pi = 0; pi < primes_cache_cnt; ++pi) {
        idx_t p = primes_cache[pi];
        for (idx_t m = p; m <= N; m += p) {
            new_phi[m] = new_phi[m] / p * (p - 1);
        }
    }

    /* free old cache and replace */
    free(phi_cache);
    phi_cache = new_phi;
    phi_cache_len = N;
}

/* public sequence function */
seq_t a(idx_t n) {
    if (n == 0)
        return 0;  /* undefined for offset 1, but return 0 for safety */

    idx_t limit = 5 * n;
    build_phi_up_to(limit);

    seq_t sum = 0;
    for (idx_t k = 1; k <= n; ++k) {
        sum += phi_cache[5 * k];
    }

    /* divide by 4; the sum is always divisible by 4 */
    return sum / (seq_t)4;
}
