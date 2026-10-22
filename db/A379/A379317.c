/*
 * A379317  Positive integers with a unique even prime index.
 * https://oeis.org/A379317
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 61, u16 n <= 61, u32 n <= 61, u64 n <= 61, u128 n <= 61,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computation: sequence of positive integers with exactly one even prime
   index in their multiset of prime indices. */

#include <stddef.h>
#include <stdint.h>
#include <stdbool.h>
#include <stdlib.h>
#include <string.h>

/*---  Dynamic storage for the sequence terms ---*/
static seq_t *cache = NULL;
static size_t cache_size = 0;
static size_t cache_capacity = 0;
static uint64_t last_candidate_checked = 0;

/*---  Dynamic storage for the list of primes ---*/
static uint64_t *primes = NULL;
static size_t primes_size = 0;
static size_t primes_capacity = 0;
static uint64_t last_prime_checked = 0;

/* Grow the cache array to accommodate at least 'needed' elements. */
static void grow_cache(size_t needed) {
    if (cache_capacity >= needed) return;
    size_t new_cap = cache_capacity ? cache_capacity * 2 : 8;
    while (new_cap < needed) new_cap <<= 1;
#ifdef __cplusplus
    seq_t *new_ptr = new seq_t[new_cap];
    for (size_t i = 0; i < cache_size; ++i) new_ptr[i] = cache[i];
    delete[] cache;
#else
    seq_t *new_ptr = (seq_t*)malloc(new_cap * sizeof(seq_t));
    if (cache) { memcpy(new_ptr, cache, cache_size * sizeof(seq_t)); free(cache); }
#endif
    cache = new_ptr;
    cache_capacity = new_cap;
}

/* Grow the primes array to accommodate at least 'needed' primes. */
static void grow_primes(size_t needed) {
    if (primes_capacity >= needed) return;
    size_t new_cap = primes_capacity ? primes_capacity * 2 : 8;
    while (new_cap < needed) new_cap <<= 1;
#ifdef __cplusplus
    uint64_t *new_ptr = new uint64_t[new_cap];
    for (size_t i = 0; i < primes_size; ++i) new_ptr[i] = primes[i];
    delete[] primes;
#else
    uint64_t *new_ptr = (uint64_t*)malloc(new_cap * sizeof(uint64_t));
    if (primes) { memcpy(new_ptr, primes, primes_size * sizeof(uint64_t)); free(primes); }
#endif
    primes = new_ptr;
    primes_capacity = new_cap;
}

/* Ensure that all primes up to 'limit' are present in the primes array. */
static void ensure_primes_up_to(uint64_t limit) {
    if (limit <= last_prime_checked) return;
    for (uint64_t cand = last_prime_checked + 1; cand <= limit; ++cand) {
        if (cand < 2) continue;
        bool is_prime = true;
        for (size_t i = 0; i < primes_size; ++i) {
            uint64_t p = primes[i];
            if (p * p > cand) break;
            if (cand % p == 0) { is_prime = false; break; }
        }
        if (is_prime) {
            grow_primes(primes_size + 1);
            primes[primes_size++] = cand;
        }
    }
    last_prime_checked = limit;
}

/* Test whether a given n has exactly one even prime index (counting multiplicity). */
static bool qualifies(uint64_t n) {
    if (n < 2) return false;
    ensure_primes_up_to(n);

    uint64_t temp = n;
    size_t even_occ = 0;

    for (size_t i = 0; i < primes_size && primes[i] * primes[i] <= temp; ++i) {
        uint64_t p = primes[i];
        if (temp % p == 0) {
            size_t exp = 0;
            while (temp % p == 0) {
                temp /= p;
                ++exp;
            }
            /* index of p is i+1 (1‑based) */
            if (((i + 1) & 1) == 0) {
                even_occ += exp;
                if (even_occ > 1) return false;  /* early exit */
            }
        }
    }

    if (temp > 1) {
        /* temp is prime > sqrt(original n) */
        size_t lo = 0, hi = primes_size;
        while (lo < hi) {
            size_t mid = (lo + hi) >> 1;
            if (primes[mid] < temp) lo = mid + 1;
            else hi = mid;
        }
        size_t pos = lo;
        if (pos == primes_size || primes[pos] != temp) {
            /* prime not yet in list – extend list */
            ensure_primes_up_to(temp);
            lo = 0; hi = primes_size;
            while (lo < hi) {
                size_t mid = (lo + hi) >> 1;
                if (primes[mid] < temp) lo = mid + 1;
                else hi = mid;
            }
            pos = lo;
        }
        if (((pos + 1) & 1) == 0) {
            even_occ += 1;
            if (even_occ > 1) return false;
        }
    }

    return even_occ == 1;
}

/*---  The required sequence function ---*/
seq_t a(idx_t n) {
    if (n == 0) return (seq_t)0;          /* by convention – offset starts at 1 */
    if (cache_size < (size_t)n) {
        uint64_t candidate = (last_candidate_checked + 1 > 3) ? last_candidate_checked + 1 : 3;
        while (cache_size < (size_t)n) {
            if (qualifies(candidate)) {
                grow_cache(cache_size + 1);
                cache[cache_size++] = (seq_t)candidate;
            }
            ++candidate;
        }
        last_candidate_checked = candidate - 1;
    }
    return cache[n - 1];
}
