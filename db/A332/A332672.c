/*
 * A332672  Number of non-unimodal permutations of a multiset whose
 *          multiplicities are the prime indices of n.
 * https://oeis.org/A332672
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = number of non‑unimodal permutations of the multiset whose
 *            multiplicities are the prime indices of n
 * Range:     u8 n <= 20, u16 n <= 37, u32 n <= 71, u64 n <= 71, u128 n <= 71,
 *            big any n
 * Helpers:   factorial_upto, sort_descending
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Implementation uses the combinatorial description:
      - total permutations = N! / Π m_i!   where m_i are the multiplicities
      - unimodal permutations = Π_{i=1}^{k-1} (m_i + 1)
      - a(n) = total - unimodal
   The multiset is constructed by taking the prime indices of n
   (each repeated by the exponent of the factor) and sorting them
   in descending order; these sorted values become the multiplicities
   of the distinct elements 1,2,…,k.  */

#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>
#include <math.h>
#include "../utils/factorial_upto.h"
#include "../utils/sort_descending.h"

/* static prime cache */
static uint32_t *prime_cache      = NULL;   /* array of primes */
static size_t    prime_cache_cnt  = 0;      /* number of primes in cache */
static idx_t     prime_cache_lim  = 0;      /* current upper limit of cache */

/* generate primes up to `limit` (inclusive) and store them in the cache */
static void ensure_primes(idx_t limit)
{
    if (limit <= prime_cache_lim) return;
    /* simple sieve of Eratosthenes */
    bool *is_prime = (bool*)malloc((size_t)(limit + 1));
    if (!is_prime) return;                 /* out of memory – unlikely */
    for (size_t i = 0; i <= limit; ++i) is_prime[i] = true;
    is_prime[0] = is_prime[1] = false;
    for (size_t i = 2; i * i <= limit; ++i) {
        if (is_prime[i]) {
            for (size_t j = i * i; j <= limit; j += i) is_prime[j] = false;
        }
    }
    /* count primes */
    size_t cnt = 0;
    for (size_t i = 2; i <= limit; ++i) if (is_prime[i]) ++cnt;
    uint32_t *new_primes = (uint32_t*)malloc(cnt * sizeof(uint32_t));
    if (!new_primes) { free(is_prime); return; }
    size_t idx = 0;
    for (size_t i = 2; i <= limit; ++i) if (is_prime[i]) new_primes[idx++] = (uint32_t)i;
    /* replace old cache */
    if (prime_cache) free(prime_cache);
    prime_cache   = new_primes;
    prime_cache_cnt = cnt;
    prime_cache_lim = limit;
    free(is_prime);
}

/* binary search: return 1‑based index of prime p in the cache,
   -1 if not found (should not happen for valid input). */
static int prime_index(uint32_t p)
{
    size_t lo = 0, hi = prime_cache_cnt;
    while (lo < hi) {
        size_t mid = (lo + hi) >> 1;
        if (prime_cache[mid] == p) return (int)(mid + 1);
        if (prime_cache[mid] < p) lo = mid + 1;
        else hi = mid;
    }
    return -1; /* should not occur */
}

/* main sequence routine */
seq_t a(idx_t n)
{
    if (n < 1) return 0;   /* out of defined range */

    /* Estimate upper bound for prime generation */
    idx_t sqrt_n = (idx_t)(sqrt((double)n) + 1);
    ensure_primes(sqrt_n);

    /* array to hold the prime indices of n (with multiplicity) */
    int indices[128];
    size_t k = 0;
    idx_t temp = n;
    for (size_t i = 0; i < prime_cache_cnt; ++i) {
        uint32_t p = prime_cache[i];
        if ((idx_t)p * (idx_t)p > temp) break;
        while (temp % p == 0) {
            temp /= p;
            indices[k++] = prime_index(p);
        }
    }
    if (temp > 1) {                    /* remaining prime factor */
        ensure_primes(temp);
        indices[k++] = prime_index((uint32_t)temp);
    }

    if (k == 0) return 0;              /* n == 1 */

    /* Sort the indices in descending order → multiplicities m_i */
    sort_descending(indices, k);

    /* total size S = sum of multiplicities */
    idx_t S = 0;
    for (size_t i = 0; i < k; ++i) S += indices[i];

    /* total number of permutations: S! / Π m_i! */
    seq_t total = factorial_upto(S);
    for (size_t i = 0; i < k; ++i) {
        seq_t denom = factorial_upto(indices[i]);
        total /= denom;        /* exact division */
    }

    /* number of unimodal permutations: Π_{i=1}^{k-1} (m_i + 1) */
    seq_t unimodal = 1;
    for (size_t i = 0; i + 1 < k; ++i) {
        unimodal *= (seq_t)(indices[i] + 1);
    }

    /* non‑unimodal count */
    seq_t result = total - unimodal;
    return result;
}
