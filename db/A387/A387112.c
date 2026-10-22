/*
 * A387112  Numbers with (strictly) choosable initial intervals of prime
 *          indices.
 * https://oeis.org/A387112
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 67, u16 n <= 67, u32 n <= 67, u64 n <= 67, u128 n <= 67,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computes a(n): the nth positive integer whose multiset of prime indices
   yields a choosable family of initial intervals. */
#include <stdint.h>
#include <stdbool.h>
#include <stdlib.h>

/* External integer types provided by the build */
extern seq_t a(idx_t n);

/*--- Prime infrastructure --------------------------------------------------*/
static uint32_t *primes_arr = NULL;      /* list of primes (1‑based)   */
static size_t primes_cnt   = 0;
static uint32_t *prime_index_arr = NULL; /* prime_index_arr[p] = index of prime p, 0 if not prime  */
static uint64_t prime_limit = 0;        /* current sieve limit */

/* builds a sieve up to at least 'lim' */
static void build_primes_for_limit(uint64_t lim) {
    if (prime_limit >= lim) return;
    while (prime_limit < lim) prime_limit = prime_limit * 2 + 1;

    uint8_t *is_prime = (uint8_t*)calloc(prime_limit + 1, 1);
    if (!is_prime) exit(1);
    is_prime[0] = is_prime[1] = 0;
    for (uint64_t i = 2; i <= prime_limit; ++i) is_prime[i] = 1;

    for (uint64_t i = 2; i * i <= prime_limit; ++i) {
        if (is_prime[i]) {
            for (uint64_t j = i * i; j <= prime_limit; j += i) is_prime[j] = 0;
        }
    }

    /* Count primes */
    size_t cnt = 0;
    for (uint64_t i = 2; i <= prime_limit; ++i)
        if (is_prime[i]) ++cnt;

    free(primes_arr);
    free(prime_index_arr);
    primes_arr = (uint32_t*)malloc(cnt * sizeof(uint32_t));
    prime_index_arr = (uint32_t*)calloc(prime_limit + 1, sizeof(uint32_t));

    size_t idx = 0;
    for (uint64_t i = 2; i <= prime_limit; ++i) {
        if (is_prime[i]) {
            primes_arr[idx] = (uint32_t)i;
            prime_index_arr[i] = (uint32_t)(idx + 1); /* 1‑based index */
            ++idx;
        }
    }
    primes_cnt = cnt;
    free(is_prime);
}

/* ensure that primes are built up to cover 'x' (and sufficiently larger) */
static inline void ensure_prime_limit_for_candidate(uint64_t x) {
    uint64_t lim = x * 2;
    if (prime_limit < lim) build_primes_for_limit(lim);
}

/*--- Choosability test -----------------------------------------------------*/
static bool is_choosable_uint64(uint64_t n) {
    if (n == 1) return true;

    ensure_prime_limit_for_candidate(n);
    uint64_t tmp = n;
    uint32_t indices[64];   /* enough for any n up to 64 bits */
    size_t sz = 0;

    for (size_t ip = 0; ip < primes_cnt; ++ip) {
        uint32_t p = primes_arr[ip];
        if ((uint64_t)p * (uint64_t)p > tmp) break;
        while (tmp % p == 0) {
            indices[sz++] = prime_index_arr[p];
            tmp /= p;
        }
    }
    if (tmp > 1) {   /* remaining prime factor */
        if (prime_index_arr[(uint32_t)tmp] == 0)
            ensure_prime_limit_for_candidate(tmp);
        indices[sz++] = prime_index_arr[(uint32_t)tmp];
    }

    /* simple insertion sort */
    for (size_t i = 1; i < sz; ++i) {
        uint32_t v = indices[i];
        size_t j = i;
        while (j > 0 && indices[j - 1] > v) {
            indices[j] = indices[j - 1];
            --j;
        }
        indices[j] = v;
    }

    for (size_t i = 0; i < sz; ++i)
        if (indices[i] < (uint32_t)(i + 1))
            return false;
    return true;
}

/*--- Sequence cache --------------------------------------------------------*/
static seq_t *seq_cache = NULL;
static size_t seq_cap = 0, seq_len = 0;
static uint64_t next_candidate = 1;
static bool cache_initialized = false;

seq_t a(idx_t n) {
    if (n == 0) return (seq_t)0;   /* offset exception; not used */
    /* initialize cache on first call */
    if (!cache_initialized) {
        seq_cap = 16;
        seq_cache = (seq_t*)malloc(seq_cap * sizeof(seq_t));
        seq_len = 0;
        next_candidate = 1;
        cache_initialized = true;
    }

    while (seq_len < (size_t)n) {
        if (is_choosable_uint64(next_candidate)) {
            if (seq_len == seq_cap) {
                seq_cap *= 2;
                seq_cache = (seq_t*)realloc(seq_cache, seq_cap * sizeof(seq_t));
            }
            seq_cache[seq_len++] = (seq_t)next_candidate;
        }
        ++next_candidate;
    }

    return seq_cache[n - 1];
}
