/*
 * A343973  Primes p such that both p-1 and p+1 are in A093641.
 * https://oeis.org/A343973
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, more
 * Formula:   not stated
 * Range:     u8 n <= 9, u16 n <= 10, u32 n <= 10, u64 n <= 10, u128 n <= 10,
 *            big any n
 * Helpers:   is_prime_big, mul_small, pow2
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* A343973: primes p such that both p-1 and p+1 contain at most one odd prime factor.
   The routine generates primes of the form 3*2^k±1, checks the condition,
   and keeps the results in a memoised cache so that calls with any order
   are handled efficiently. */
#include <stdlib.h>
#include <stdbool.h>
#include "../utils/pow2.h"
#include "../utils/mul_small.h"
#include "../utils/is_prime_big.h"

/* Helper: return true if x has at most one odd prime factor (counted with multiplicity) */
static bool has_at_most_one_odd_prime_factor(seq_t x)
{
    /* Remove factors of 2 */
    while ((x & 1) == 0) {
        x /= 2;
    }
    /* x is now odd. It must be 1 or a prime to satisfy the condition. */
    if (x == 1) {
        return true;
    }
    return is_prime_big(x);
}

/* Helper: true if p is a prime satisfying the A343973 condition (tested on p-1 and p+1) */
static bool candidate_ok(seq_t p)
{
    if (!is_prime_big(p)) {
        return false;
    }
    if (!has_at_most_one_odd_prime_factor(p - 1)) {
        return false;
    }
    if (!has_at_most_one_odd_prime_factor(p + 1)) {
        return false;
    }
    return true;
}

/* Add a term to the cache, expanding capacity if needed */
static void add_term(seq_t **cache, size_t *len, size_t *cap, seq_t val)
{
    if (*len == *cap) {
        *cap *= 2;
        *cache = (seq_t *)realloc(*cache, *cap * sizeof(seq_t));
    }
    (*cache)[*len] = val;
    (*len)++;
}

seq_t a(idx_t n)
{
    /* Static state: memoised sequence and next exponent to test */
    static seq_t *cache = NULL;   /* array of generated terms */
    static size_t cache_len = 0;  /* number of entries currently stored */
    static size_t cache_cap = 0;  /* allocated capacity */
    static idx_t next_k = 2;      /* start test at k = 2 (k=1 already covered) */

    /* Initialise cache lazily, inserting the first four known terms */
    if (cache == NULL) {
        cache_cap = 16;
        cache = (seq_t *)malloc(cache_cap * sizeof(seq_t));
        cache[0] = (seq_t)2;
        cache[1] = (seq_t)3;
        cache[2] = (seq_t)5;
        cache[3] = (seq_t)7;
        cache_len = 4;
    }

    /* Generate new terms until we have at least n entries */
    while (cache_len < n) {
        seq_t pow2_k = pow2((idx_t)next_k);
        seq_t base  = mul_small(pow2_k, 3);          /* 3*2^k */

        /* Candidate p = 3*2^k - 1 */
        seq_t p_minus  = base - 1;
        if (candidate_ok(p_minus)) {
            add_term(&cache, &cache_len, &cache_cap, p_minus);
        }

        /* Candidate p = 3*2^k + 1 */
        seq_t p_plus   = base + 1;
        if (candidate_ok(p_plus)) {
            add_term(&cache, &cache_len, &cache_cap, p_plus);
        }

        next_k++;   /* move to next exponent */
    }

    return cache[n - 1];  /* 1‑based indexing */
}
