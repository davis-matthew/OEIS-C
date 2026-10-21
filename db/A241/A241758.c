/*
 * A241758  Smallest prime in representation 2*A241757(n) by sum of two
 *          primes, the adding of which in binary requires only one carry.
 * https://oeis.org/A241758
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) = smallest prime p such that p +(2*A241757(n)-p) has
 *            exactly one binary carry and p+ (2*A241757(n)-p) = 2*A241757(n),
 *            where A241757(n) is the nth even integer that can be written as
 *            a sum of two odd primes with exactly one binary carry in their
 *            addition. The sequence is the smallest such pri...
 * Range:     u8 n <= 108, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   carry_count
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stddef.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include "../utils/carry_count.h"

/* Upper bound for prime sieve.  Sufficient for n up to about 10000. */
static const idx_t SIEVE_LIMIT = 10000000u;

/* Prime sieve data. */
static uint8_t *is_prime = NULL;   /* is_prime[i] == 1 ↔ i is prime */
static idx_t   is_prime_limit = 0;
static idx_t *prime_list   = NULL; /* array of all primes up to limit */
static size_t prime_count  = 0;
static bool   primes_sieved = false;

/* Build the sieve of Eratosthenes and prime list up to 'limit'. */
static void sieve_primes(idx_t limit)
{
    if (primes_sieved) return;
    is_prime = (uint8_t*)malloc((limit + 1) * sizeof(uint8_t));
    if (!is_prime) return;              /* allocation failed */
    memset(is_prime, 1, limit + 1);
    is_prime[0] = is_prime[1] = 0;

    for (idx_t p = 2; p * p <= limit; ++p) {
        if (is_prime[p]) {
            for (idx_t m = p * p; m <= limit; m += p)
                is_prime[m] = 0;
        }
    }

    /* Create prime list. */
    size_t est = limit / 10 + 100;
    prime_list = (idx_t*)malloc(est * sizeof(idx_t));
    if (!prime_list) return;            /* allocation failed */
    prime_count = 0;
    for (idx_t v = 2; v <= limit; ++v) {
        if (is_prime[v]) {
            if (prime_count == est) {
                est <<= 1;
                prime_list = (idx_t*)realloc(prime_list, est * sizeof(idx_t));
                if (!prime_list) return;
            }
            prime_list[prime_count++] = v;
        }
    }

    is_prime_limit = limit;
    primes_sieved = true;
}

/* Find the smallest prime p < q such that p + q = x,
   p and q are odd primes (except x == 4, where 2+2 is allowed),
   and the binary addition of p and q has exactly one carry.
   If such a pair exists, store the smallest p in *minp and
   return true; otherwise return false. */
static inline bool find_min_prime_for_sum(idx_t x, idx_t *minp)
{
    if (x == 4) {                 /* the only case that uses the prime 2 */
        if (2 * 2 == x) {
            *minp = 2u;
            return true;
        }
    }

    /* Only even x > 4 can be expressed as sum of two odd primes. */
    if (x <= 4 || (x & 1u)) return false;

    for (size_t i = 0; i < prime_count; ++i) {
        idx_t p = prime_list[i];
        if (p < 3u) continue;     /* skip 2 for x > 4 */
        if ((idx_t)(p * 2u) > x) break;   /* p > x/2 */
        idx_t q = x - p;
        if (q <= p) continue;     /* require distinct primes (ensures minimal p) */
        if (q > is_prime_limit) break;    /* out of sieve range */
        if (!is_prime[q]) continue;
        if (carry_count(p, q) == 1u) {
            *minp = p;
            return true;
        }
    }
    *minp = 0;
    return false;
}

/* Memoised cache for the sequence. */
static seq_t *seq_cache = NULL;   /* array of computed values */
static idx_t  seq_len   = 0;       /* number of values stored */
static size_t seq_cap   = 0;       /* capacity of seq_cache */
static idx_t  last_sum  = 0;       /* last integer processed */

/* Ensure seq_cache can hold at least 'needed' elements. */
static void seq_ensure_capacity(idx_t needed)
{
    if (needed <= (idx_t)seq_cap) return;
    size_t new_cap = seq_cap ? seq_cap * 2 : 64;
    while (new_cap < (size_t)needed) new_cap <<= 1;
    seq_cache = (seq_t*)realloc(seq_cache, new_cap * sizeof(seq_t));
    seq_cap   = new_cap;
}

/* Public function required by the harness. */
seq_t a(idx_t n)
{
    if (n == 0) return 0;          /* undefined – return 0 for safety */

    if (!primes_sieved) sieve_primes(SIEVE_LIMIT);

    while (seq_len < n) {          /* generate sequence up to n */
        idx_t candidate = last_sum + 1u;
        idx_t minp = 0;

        /* Search for the next integer with the required property. */
        while (true) {
            if (find_min_prime_for_sum(candidate, &minp)) break;
            ++candidate;
        }

        seq_ensure_capacity(seq_len + 1);
        seq_cache[seq_len++] = (seq_t)minp;
        last_sum = candidate;
    }

    return seq_cache[n - 1];
}
