/*
 * A373473  Numbers k such that A001414(k) and A059975(k) are both multiples
 *          of 3, where A001414 and A059975 are fully additive with a(p) = p
 *          and a(p) = p-1, respectively.
 * https://oeis.org/A373473
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 29, u16 n <= 7405, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence: numbers k such that
   A001414(k) = sum_{p^e||k} e·p  is a multiple of 3, and
   A059975(k) = sum_{p^e||k} e·(p-1) is a multiple of 3.
   The value a(n) is the nth positive integer satisfying the above
   conditions (a(1)=1).  The implementation tests each integer in order,
   memoizing results so that calls can arrive in any order. */

#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>

static seq_t *cache = NULL;     /* cached sequence values */
static size_t cache_cap = 0;    /* current capacity of cache */
static size_t cache_len = 0;    /* number of values stored */

/* Prime storage */
static uint32_t *primes = NULL;   /* array of primes */
static size_t primes_cap = 0;     /* capacity of primes array */
static size_t primes_len = 0;     /* number of primes stored */
static uint32_t next_check = 3;   /* next odd number to test for primality */

/* Candidate to test next (first is 1) */
static uint64_t next_candidate = 1;

/* Allocate or grow an array of seq_t values.  cap distinguishes whether
   the capacity is being increased. */
static void grow_cache(size_t new_cap) {
    seq_t *newp = (seq_t*)realloc(cache, new_cap * sizeof(seq_t));
    if (!newp) abort();   /* fail hard */
    cache = newp;
    cache_cap = new_cap;
}

/* Allocate or grow the prime array. */
static void grow_primes(size_t new_cap) {
    uint32_t *newp = (uint32_t*)realloc(primes, new_cap * sizeof(uint32_t));
    if (!newp) abort();
    primes = newp;
    primes_cap = new_cap;
}

/* Test whether x is prime using the current list of primes. */
static bool is_prime(uint32_t x) {
    if (x < 2) return false;
    for (size_t i = 0; i < primes_len; ++i) {
        uint32_t p = primes[i];
        if ((uint64_t)p * p > x) break;
        if (x % p == 0) return false;
    }
    return true;
}

/* Ensure we have all primes up to sqrt(limit).  The caller provides
   an upper bound; we grow the prime list until the largest prime in
   the list squared exceeds the bound. */
static void ensure_primes_upto(uint64_t limit) {
    while ((uint64_t)next_check * next_check <= limit) {
        if (is_prime(next_check)) {
            if (primes_len == primes_cap) grow_primes(primes_cap ? primes_cap * 2 : 16);
            primes[primes_len++] = next_check;
        }
        next_check += 2;
    }
}

/* Initialize prime list with the first prime. */
static void init_primes(void) {
    if (primes_len == 0) {
        grow_primes(16);
        primes[primes_len++] = 2;
        next_check = 3;
    }
}

/* Test whether integer k satisfies the sequence condition. */
static bool ok_k(uint64_t k) {
    if (k == 1) return true;   /* base case; Omega = 0, sum = 0 */
    uint64_t tmp = k;
    uint64_t omega = 0;
    uint64_t sum = 0;

    /* Ensure primes up to sqrt(k) are available */
    ensure_primes_upto(k);

    for (size_t i = 0; i < primes_len; ++i) {
        uint32_t p = primes[i];
        if ((uint64_t)p * p > tmp) break;
        while (tmp % p == 0) {
            tmp /= p;
            ++omega;
            sum += p;
        }
    }
    if (tmp > 1) {   /* remaining prime factor */
        ++omega;
        sum += tmp;
    }
    return (omega % 3 == 0) && (sum % 3 == 0);
}

/* Append a new value to the cache. */
static void append_cache(seq_t val) {
    if (cache_len == cache_cap) {
        size_t new_cap = cache_cap ? cache_cap * 2 : 16;
        grow_cache(new_cap);
    }
    cache[cache_len++] = val;
}

/* Main function. */
seq_t a(idx_t n) {
    if (n == 0) return 0;   /* undefined, but avoid UB */

    /* One–time initialisation */
    if (!cache) {
        cache_cap = 16;
        cache = (seq_t*)malloc(cache_cap * sizeof(seq_t));
        if (!cache) abort();
        cache_len = 0;

        init_primes();
    }

    /* Generate sequence values until we have at least n of them */
    while (cache_len < (size_t)n) {
        if (ok_k(next_candidate)) {
            append_cache((seq_t)next_candidate);
        }
        ++next_candidate;
    }

    return cache[n-1];
}
