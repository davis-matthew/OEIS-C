/*
 * A269111  a(n) = length of the repeating part of row n of A288097.
 * https://oeis.org/A269111
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, hard, more
 * Formula:   not stated
 * Range:     u8 n <= 14, u16 n <= 14, u32 n <= 14, u64 n <= 14, u128 n <= 14,
 *            big any n
 * Validated: partial, pass, 14 terms through n = 14 (2026-11-03)
 * Helpers:   is_prime_u128, is_prime_u64, powmod
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
#include <stdlib.h>
#include <limits.h>
#include <string.h>
#include "../utils/is_prime_u128.h"
#include "../utils/is_prime_u64.h"
#include "../utils/powmod.h"

/* -----------------  Generic primality test ------------------------ */
static bool isPrimeGeneric(seq_t n)
{
    /* all seq_t are unsigned; use the smallest type that fits */
    if ((seq_t)(ULLONG_MAX) >= n) {
        return is_prime_u64((uint64_t)n);
    }
#if defined(__SIZEOF_INT128__)
    return is_prime_u128((__uint128_t)n);
#else
    /* fallback: if seq_t > 64‑bit but no 128‑bit support,
       order the test as simple trial division up to sqrt */
    seq_t limit = 0;
    for (seq_t i = 2; i*i <= n; ++i) {
        if (n % i == 0) return false;
    }
    return true;
#endif
}

/* Return the k‑th prime (k ≥ 1).  On first call the prime list
   is built incrementally using trial division.  The list is
   stored in a static array which is expanded as necessary.   */
static uint32_t get_prime(idx_t k)
{
    static uint32_t *pr = NULL;     /* array of primes */
    static size_t sz   = 0;         /* number of primes stored */
    static size_t cap  = 0;         /* allocated capacity */
    static uint32_t cand = 3;       /* candidate after 2 */

    /* initialise the prime table on first use */
    if (sz == 0) {
        cap = 8;
        pr = (uint32_t *)realloc(pr, cap * sizeof(uint32_t));
        if (!pr) abort();
        sz = 0;
    }

    /* grow the array until we have at least k primes */
    while (sz < (size_t)k) {
        if (sz == 0) {                /* add the first prime */
            pr[sz++] = 2;
            continue;
        }

        /* find the next prime candidate */
        uint32_t p = cand;
        bool ok = true;
        for (size_t i = 0; i < sz; ++i) {
            uint32_t prime = pr[i];
            if ((uint64_t)prime * prime > (uint64_t)p) break;
            if (p % prime == 0) { ok = false; break; }
        }

        if (!ok) {                    /* not prime – keep searching */
            cand += 2;                /* only odd numbers remain */
            continue;
        }

        /* prime found – store it, grow table if needed */
        if (sz == cap) {
            cap <<= 1;
            pr = (uint32_t *)realloc(pr, cap * sizeof(uint32_t));
            if (!pr) abort();
        }
        pr[sz++] = p;
        cand = p + 2;
    }

    return pr[(size_t)k - 1];
}

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
        if (powmod((seq_t)base, (uint64_t)candidate - 1, mod) == 1) {
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
