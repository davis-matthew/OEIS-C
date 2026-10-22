/*
 * A359371  Nonmultiples of 4 that have an even number of prime factors (with
 *          multiplicity).
 * https://oeis.org/A359371
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 94, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computes A359371: a(n) is the nth positive integer that is NOT a
   multiple of 4 and has an even number of prime factors (counted with
   multiplicity).  The sequence starts with
     a(1)=1, a(2)=6, a(3)=9, …
   (OEIS offset 1). */

#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>
#include <stddef.h>   /* for size_t */

/* Small prime sieve up to MAX_PRIME (triple limit sufficient for
   numbers we generate).  Larger values incur more memory/time, but for
   the intended n ≤ 10 000 MAX_PRIME=100 000 is plenty. */
#define MAX_PRIME 100000
static uint32_t primes[MAX_PRIME + 1];
static size_t prime_cnt = 0;
static bool primes_inited = false;

static void init_primes(void) {
    if (primes_inited) return;
    primes_inited = true;
    bool marked[MAX_PRIME + 1] = { false };
    prime_cnt = 0;
    for (uint32_t i = 2; i <= MAX_PRIME; ++i) {
        if (!marked[i]) {
            primes[prime_cnt++] = i;
            if ((uint64_t)i * i <= MAX_PRIME) {
                for (uint32_t j = i * i; j <= MAX_PRIME; j += i)
                    marked[j] = true;
            }
        }
    }
}

/* Return true iff the count Ω(n) (prime factors with multiplicity)
   is even.  Uses the prime table from init_primes(). */
static bool even_omega(uint64_t n) {
    bool parity = false;                       /* false → even, true → odd */
    uint64_t m = n;
    for (size_t i = 0; i < prime_cnt && (uint64_t)primes[i] * primes[i] <= m; ++i) {
        uint32_t p = primes[i];
        while (m % p == 0) {
            parity = !parity;
            m /= p;
        }
    }
    if (m > 1) parity = !parity;                /* remaining prime factor */
    return !parity;                            /* true if even Ω(n) */
}

/* Cache of computed terms.  The array holds seq_t, which may be an
   integer type or an arbitrary‑precision class; memset/realloc
   handling is allowed because sizeof(seq_t) is known at compile time
   (C++) or is a typedef (C). */
static seq_t *cache = NULL;
static size_t cache_len = 0;
static size_t cache_cap = 0;
static idx_t next_candidate = 1;

/* Append a term to the cache, expanding the array as needed. */
static void cache_append(seq_t val) {
    if (cache_len >= cache_cap) {
        size_t new_cap = cache_cap ? cache_cap * 2 : 32;
        cache = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        cache_cap = new_cap;
    }
    cache[cache_len++] = val;
}

/* Main sequence function.  The function is self‑contained and
   correctly memoises results across arbitrary call order. */
seq_t a(idx_t n) {
    if (n == 0) return 0;                 /* undefined for n = 0 */
    init_primes();
    while (cache_len < n) {
        uint64_t cand = (uint64_t)next_candidate++;
        if (cand % 4 == 0) continue;      /* skip multiples of 4 */
        if (even_omega(cand))
            cache_append((seq_t)cand);
    }
    return cache[n - 1];
}
