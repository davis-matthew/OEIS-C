/*
 * A269111  a(n) = length of the repeating part of row n of A288097.
 * https://oeis.org/A269111
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, hard, more
 * Formula:   not stated
 * Range:     u8 n <= 14, u16 n <= 14, u32 n <= 14, u64 n <= 14, u128 n <= 14,
 *            big any n
 * Helpers:   get_prime, isPrimeGeneric, mod_pow
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*
 * Formula: a(n) = length of the repeating part of the trajectory of the
 * (prime(n)-th) row of A288097.  The trajectory is obtained by iterating
 * the function f(p) = smallest prime q with p^(q‑1) ≡ 1 (mod q^2).
 * a(n) is the cycle length of this iteration for the starting prime
 * prime(n).
 *
 * Implementation details:
 * - A small hash table caches f(p) values to avoid repeated computations.
 * - Floyd’s cycle‑finding algorithm is used to determine the cycle length
 *   with O(1) memory.
 * - For modular exponentiation we use the utility function mod_pow.
 * - Primality testing of candidate primes is done by isPrimeGeneric.
 */

#include <stddef.h>
#include <stdint.h>
#include <stdbool.h>
#include "../utils/isPrimeGeneric.h"
#include "../utils/mod_pow.h"
#include "../utils/get_prime.h"

/* ------------------------------------------------------------------ */
/* Small hash table for caching next prime values.  Key == 0 means empty. */
#define CACHE_SIZE  (1U << 20)   /* 1,048,576 entries */
static struct {
    uint32_t key;   /* base prime */
    uint32_t val;   /* next prime value */
} prime_next_cache[CACHE_SIZE];
static const uint32_t cache_mask = CACHE_SIZE - 1u;

/* Simple multiplicative hash for 32‑bit keys */
static inline uint32_t hash_key(uint32_t k) {
    return k * 1199540897u;
}

/* Table lookup: returns 0 if not found */
static uint32_t cache_lookup(uint32_t key) {
    uint32_t h = hash_key(key);
    size_t idx = h & cache_mask;
    while (prime_next_cache[idx].key) {
        if (prime_next_cache[idx].key == key) {
            return prime_next_cache[idx].val;
        }
        idx = (idx + 1) & cache_mask;
    }
    return 0;
}

/* Table insertion (overwrites existing entry if present) */
static void cache_insert(uint32_t key, uint32_t val) {
    uint32_t h = hash_key(key);
    size_t idx = h & cache_mask;
    while (prime_next_cache[idx].key) {
        if (prime_next_cache[idx].key == key) {
            prime_next_cache[idx].val = val;
            return;
        }
        idx = (idx + 1) & cache_mask;
    }
    prime_next_cache[idx].key = key;
    prime_next_cache[idx].val = val;
}

/* ------------------------------------------------------------------ */
/* Compute f(p): smallest prime q such that p^(q‑1) ≡ 1 (mod q^2). */
static uint32_t next_prime_for_base(uint32_t base) {
    uint32_t cached = cache_lookup(base);
    if (cached) return cached;

    /* Search primes in increasing order 2,3,5,7,... until condition holds */
    for (uint32_t candidate = 2;; ++candidate) {
        if (!isPrimeGeneric((seq_t)candidate))
            continue;
        seq_t mod = (seq_t)candidate * (seq_t)candidate;
        if (mod_pow((seq_t)base, (uint64_t)candidate - 1, mod) == 1) {
            cache_insert(base, candidate);
            return candidate;
        }
    }
}

/* ------------------------------------------------------------------ */
/* Main function: gives the length of the cycle for prime(n). */
seq_t a(idx_t n) {
    /* Starting prime p = prime(n) */
    uint32_t start = get_prime(n);

    /* Floyd's cycle finding */
    uint32_t tortoise = next_prime_for_base(start);
    uint32_t hare = next_prime_for_base(next_prime_for_base(start));

    while (tortoise != hare) {
        tortoise = next_prime_for_base(tortoise);
        hare = next_prime_for_base(next_prime_for_base(hare));
    }

    /* Once a meeting point is found, determine cycle length λ */
    uint32_t hare2 = next_prime_for_base(tortoise);
    uint32_t len = 1;
    while (hare2 != tortoise) {
        hare2 = next_prime_for_base(hare2);
        ++len;
    }

    return (seq_t)len;
}
