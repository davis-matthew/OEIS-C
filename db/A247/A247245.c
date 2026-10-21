/*
 * A247245  Least multiple of n that is the sum of 2 successive primes.
 * https://oeis.org/A247245
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = smallest k*n such that k*n is the sum of two consecutive
 *            primes
 * Range:     u8 n <= 28, u16 n <= 780, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>


/* ---------- static data for prime generation ---------- */
static uint64_t *prime_numbers = NULL;   /* array of primes */
static size_t prime_count = 0;           /* number of primes stored   */
static size_t prime_capacity = 0;        /* allocated capacity       */
static uint64_t prime_max = 0;           /* largest prime stored      */

/* ---------- static cache for computed a(n) values ---------- */
static seq_t *cache = NULL;      /* cache[n-1] = a(n)   */
static size_t cache_capacity = 0; /* allocated size     */

/* ---------- helper: grow capacity of prime array ---------- */
static void prime_grow(size_t needed) {
    if (prime_capacity >= needed) return;
    size_t new_cap = prime_capacity ? prime_capacity * 2 : 16;
    if (new_cap < needed) new_cap = needed;
    uint64_t *tmp = (uint64_t*)realloc(prime_numbers, new_cap * sizeof(uint64_t));
    prime_numbers = tmp;
    prime_capacity = new_cap;
}

/* ---------- helper: add a prime to the array ---------- */
static void add_prime(uint64_t p) {
    prime_grow(prime_count + 1);
    prime_numbers[prime_count++] = p;
    if (p > prime_max) prime_max = p;
}

/* ---------- initialise with the first prime ---------- */
static void init_primes(void) {
    if (prime_count == 0) {
        add_prime(2);
    }
}

/* ---------- generate primes up to a given limit ---------- */
static void generate_primes_up_to(uint64_t limit) {
    init_primes();
    if (limit <= prime_max) return;
    for (uint64_t cand = prime_max + 1; cand <= limit; ++cand) {
        if (cand > 2 && (cand & 1) == 0) continue; /* skip even numbers */
        bool is_prime = true;
        for (size_t i = 0; i < prime_count; ++i) {
            uint64_t q = prime_numbers[i];
            if (q * q > cand) break;
            if (cand % q == 0) {
                is_prime = false;
                break;
            }
        }
        if (is_prime) add_prime(cand);
    }
}

/* ---------- helper: grow cache until a(n) can be stored ---------- */
static void cache_grow(size_t needed) {
    if (cache_capacity >= needed) return;
    size_t old_cap = cache_capacity;
    size_t new_cap = cache_capacity ? cache_capacity * 2 : 16;
    if (new_cap < needed) new_cap = needed;
    seq_t *tmp = (seq_t*)realloc(cache, new_cap * sizeof(seq_t));
    cache = tmp;
    /* initialise new entries to zero (meaning 'unset') */
    for (size_t i = old_cap; i < new_cap; ++i) cache[i] = (seq_t)0;
    cache_capacity = new_cap;
}

/* ---------- main sequence function ---------- */
seq_t a(idx_t n) {
    if (n == 0) return (seq_t)0;          /* undefined for n=0, safe guard */
    size_t idx = (size_t)n - 1;

    /* initialise or enlarge cache */
    cache_grow(idx + 1);
    if (cache[idx] != (seq_t)0) return cache[idx]; /* already computed */

    /* ensure we have at least some primes */
    init_primes();

    seq_t answer = (seq_t)0;
    seq_t sn = (seq_t)n;

    while (true) {
        /* scan existing consecutive prime sums */
        for (size_t i = 0; i + 1 < prime_count; ++i) {
            seq_t sum = (seq_t)prime_numbers[i] + (seq_t)prime_numbers[i + 1];
            if (sum % sn == (seq_t)0) {
                answer = sum;
                cache[idx] = answer;   /* memoise */
                return answer;
            }
        }
        /* no sum found yet – generate more primes */
        uint64_t new_limit = prime_max * 2 + 10;   /* pick a larger bound */
        generate_primes_up_to(new_limit);
    }
}
