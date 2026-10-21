/*
 * A274225  Denominator of the ratio of consecutive prime gaps.
 * https://oeis.org/A274225
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, frac
 * Formula:   a(n) = (prime(n+1) - prime(n)) / gcd(prime(n+1) - prime(n),
 *            prime(n+2) - prime(n+1)) (denominator of the reduced ratio of
 *            consecutive prime gaps)
 * Range:     u8 n <= 1000, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   build_primes_upto, prime_limit_for_n, seq_gcd
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>
#include "../utils/build_primes_upto.h"
#include "../utils/seq_gcd.h"
#include "../utils/prime_limit_for_n.h"

static uint64_t *primes_cache = NULL;
static size_t primes_cnt = 0;

/* Ensure primes_cache contains at least 'need' primes. */
static void ensure_primes(idx_t need)
{
    if (primes_cnt >= need)
        return;

    /* Estimate an upper bound that includes the 'need'-th prime. */
    idx_t limit = prime_limit_for_n(need);

    uint64_t *tmp;
    size_t cnt;

    build_primes_upto(limit, &tmp, &cnt);

    /* The generated sieve may contain more primes than needed. */
    if (cnt < need)
    {
        /* Very unlikely for the chosen bound, but guard just in case. */
        free(tmp);
        need = cnt; /* reduce need to the available count */
    }

    if (primes_cache)
        free(primes_cache);

    primes_cache = tmp;
    primes_cnt   = cnt;
}

seq_t a(idx_t n)
{
    if (n == 0)
        return (seq_t)0;       /* undefined for offset 1; safe return */

    /* We need (n+2)-th prime, i.e., indices up to n+1 (zero‑based). */
    ensure_primes(n + 2);

    /* Prime indices: p_n = primes_cache[n-1], p_{n+1} = primes_cache[n], p_{n+2} = primes_cache[n+1] */
    uint64_t p_n   = primes_cache[(size_t)(n) - 1];
    uint64_t p_n1  = primes_cache[(size_t)(n)];
    uint64_t p_n2  = primes_cache[(size_t)(n) + 1];

    seq_t gap0 = (seq_t)(p_n1 - p_n);   /* prime(n+1) - prime(n) */
    seq_t gap1 = (seq_t)(p_n2 - p_n1);  /* prime(n+2) - prime(n+1) */

    seq_t g   = seq_gcd(gap0, gap1);
    seq_t denom = gap0 / g;              /* denominator of the reduced ratio */

    return denom;
}
