/*
 * A277688  Odd numbers k such that there is no prime p < k/2 with k - 2*p and
 *          k + 2*p both prime.
 * https://oeis.org/A277688
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   This code brute‑forces odd numbers k. It keeps a global sieve *
 *            of primes and tests k for the condition: there is no prime p <
 *            k/2 * such that k−2*p and k+2*p are both prime. The first n such
 *            k's are * cached to allow random access
 * Range:     u8 n <= 50, u16 n <= 56, u32 n <= 56, u64 n <= 56, u128 n <= 56,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>

/* Global state for the sieve */
static unsigned char *prime_is_prime = NULL;   /* 1 if prime, 0 otherwise */
static uint64_t      *prime_list      = NULL;   /* list of primes */
static size_t         primes_cnt      = 0;        /* number of primes in prime_list */
static size_t         sieve_limit     = 0;        /* maximum n for which we have a sieve */

/* Build a sieve up to 'limit' (inclusive) and populate prime_list. */
static void build_sieve(size_t limit) {
    if (prime_is_prime) free(prime_is_prime);
    prime_is_prime = (unsigned char *)malloc((limit + 1) * sizeof(unsigned char));
    if (!prime_is_prime) abort();

    /* Initially mark all as prime; 0 and 1 are not prime. */
    memset(prime_is_prime, 1, limit + 1);
    prime_is_prime[0] = 0;
    prime_is_prime[1] = 0;

    /* Eratosthenes */
    for (size_t i = 2; i * i <= limit; ++i) {
        if (prime_is_prime[i]) {
            for (size_t j = i * i; j <= limit; j += i)
                prime_is_prime[j] = 0;
        }
    }

    /* Count primes and build the list. */
    if (prime_list) free(prime_list);
    primes_cnt = 0;
    for (size_t i = 2; i <= limit; ++i)
        if (prime_is_prime[i]) ++primes_cnt;

    prime_list = (uint64_t *)malloc(primes_cnt * sizeof(uint64_t));
    if (!prime_list) abort();

    size_t idx = 0;
    for (size_t i = 2; i <= limit; ++i)
        if (prime_is_prime[i]) prime_list[idx++] = i;

    sieve_limit = limit;
}

/* Ensure that we have a sieve covering at least 'min_limit'. */
static inline void ensure_sieve(size_t min_limit) {
    if (sieve_limit >= min_limit) return;
    size_t new_limit = sieve_limit ? sieve_limit : 10;
    while (new_limit < min_limit) new_limit <<= 1;
    build_sieve(new_limit);
}

/* Cache of computed sequence terms. */
static seq_t *seq_cache = NULL;   /* holds a(1), a(2), ... */
static idx_t  seq_len   = 0;      /* number of terms stored */
static idx_t  seq_cap   = 0;      /* allocated capacity */

/* Append a value to the cache. */
static void cache_append(seq_t val) {
    if (seq_len == seq_cap) {
        size_t new_cap = seq_cap ? seq_cap * 2 : 8;
        seq_cache = (seq_t *)realloc(seq_cache, new_cap * sizeof(seq_t));
        if (!seq_cache) abort();
        seq_cap = new_cap;
    }
    seq_cache[seq_len++] = val;
}

/* Return true if odd number k satisfies the sequence condition. */
static bool qualifies(uint64_t k) {
    /* Iterate over primes p < k/2. */
    for (size_t i = 0; i < primes_cnt; ++i) {
        uint64_t p = prime_list[i];
        if (p * 2 >= k) break;          /* p >= k/2 */

        uint64_t a = k - 2 * p;
        uint64_t b = k + 2 * p;

        /* Check primality of a and b. */
        if (prime_is_prime[a] && prime_is_prime[b]) {
            return false; /* found a witness, k is excluded */
        }
    }
    return true; /* no witness found */
}

/* Generate sequence terms up to index 'n'. */
static void generate_up_to(idx_t n) {
    static uint64_t next_candidate = 1; /* candidate to test */

    while (seq_len < n) {
        /* Ensure the sieve covers up to 2*next_candidate for a,b checks. */
        ensure_sieve(2 * next_candidate + 10);

        if (next_candidate & 1ULL) { /* only odd numbers can be in the sequence */
            if (qualifies(next_candidate))
                cache_append((seq_t)next_candidate);
        }
        ++next_candidate;
    }
}

/* Public function as required. */
seq_t a(idx_t n) {
    if (n == 0) return 0;            /* undefined for n=0; return 0 safely */
    generate_up_to(n);
    return seq_cache[n - 1];
}
