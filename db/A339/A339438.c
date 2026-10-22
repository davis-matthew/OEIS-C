/*
 * A339438  Composite numbers k such that k + A339436(k) is prime.
 * https://oeis.org/A339438
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = composite number k such that the sum of the prefix
 *            products and suffix products of the prime factors of k is prime.
 *            If k has prime factorization p1 <= p2 <= ... <= pm
 *            (multiplicities included) then A339436(k) = p1 + p1*p2 + ... +
 *            p1*...*pm + p2*...*pm + ... + pm The sequence contains all c...
 * Range:     u8 n <= 43, u16 n <= 4094, u32 n <= 6180, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   _floor_isqrt_u64, is_prime, is_prime_big
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stddef.h>
#include <stdbool.h>
#include <stdlib.h>

/* Utility headers supplied by the build framework. */
#include "../utils/is_prime.h"          /* bool is_prime(idx_t x);              */
#include "../utils/is_prime_big.h"      /* bool is_prime_big(seq_t x);          */
#include "../utils/_floor_isqrt_u64.h"  /* idx_t _floor_isqrt_u64(idx_t n);     */

#if defined(__cplusplus)
/* C++ casts for realloc. */
#define RELOC_PTR(ptr, type, new_cap) (ptr) = static_cast<type *>(realloc((ptr), (new_cap) * sizeof(type)))
#else
#define RELOC_PTR(ptr, type, new_cap) (ptr) = ((type *)realloc((ptr), (new_cap) * sizeof(type)))
#endif

/* ---------- Prime list construction (incremental) ---------- */
static uint64_t *prime_list   = NULL;  /* sorted list of primes          */
static size_t   prime_len    = 0;      /* number of primes in the list   */
static size_t   prime_cap    = 0;      /* current capacity of prime_list */
static uint64_t primes_last_checked = 1;/* largest number checked so far  */

/* Append a prime to the prime list, growing the array if needed. */
static void append_prime(uint64_t p) {
    if (prime_len == prime_cap) {
        prime_cap = prime_cap ? prime_cap * 2 : 16;
        RELOC_PTR(prime_list, uint64_t, prime_cap);
    }
    prime_list[prime_len++] = p;
}

/* Ensure that all primes up to 'limit' are present in prime_list.
   The list is built incrementally; only numbers after the last checked
   point are tested. */
static void ensure_primes_up_to(uint64_t limit) {
    if (limit <= primes_last_checked) return;
    for (uint64_t cand = primes_last_checked + 1; cand <= limit; ++cand) {
        bool prime = true;
        for (size_t i = 0; i < prime_len; ++i) {
            uint64_t p = prime_list[i];
            if (p > cand / p) break;      /* p^2 > cand */
            if (cand % p == 0) {
                prime = false;
                break;
            }
        }
        if (prime) append_prime(cand);
    }
    primes_last_checked = limit;
}

/* ---------- Sequence cache ---------- */
static seq_t *seq_cache = NULL; /* stored terms */
static size_t seq_len   = 0;    /* number of cached terms */
static size_t seq_cap   = 0;    /* current cache capacity */

/* Append a newly found term to the sequence cache. */
static void seq_append(seq_t val) {
    if (seq_len == seq_cap) {
        seq_cap = seq_cap ? seq_cap * 2 : 16;
        RELOC_PTR(seq_cache, seq_t, seq_cap);
    }
    seq_cache[seq_len++] = val;
}

/* ---------- Main function ---------- */
seq_t a(idx_t n) {
    if (n == 0) return 0;   /* undefined below offset, return 0 */

    /* First composite number is 4. */
    static idx_t next_candidate = 4;

    while (seq_len < (size_t)n) {
        /* Prime factorization requires primes up to sqrt(candidate). */
        uint64_t limit = _floor_isqrt_u64((idx_t)next_candidate);
        if (limit < 2) limit = 2;
        ensure_primes_up_to(limit);

        /* Skip primes. */
        if (!is_prime((idx_t)next_candidate)) {
            /* Factorize the candidate. */
            uint64_t temp = (uint64_t)next_candidate;
            uint64_t factors[64];
            size_t factor_count = 0;

            for (size_t i = 0; i < prime_len; ++i) {
                uint64_t p = prime_list[i];
                if (p > temp / p) break;          /* p^2 > temp */
                while (temp % p == 0) {
                    if (factor_count < 64) {
                        factors[factor_count++] = p;
                    }
                    temp /= p;
                }
            }
            if (temp > 1 && factor_count < 64) {
                factors[factor_count++] = temp; /* remaining prime factor */
            }

            /* Compute S(k) = prefix products + suffix products. */
            seq_t sum   = 0;
            seq_t prod  = 1;

            for (size_t i = 0; i < factor_count; ++i) {
                prod = prod * (seq_t)factors[i];
                sum  = sum  + prod;
            }

            seq_t prod_suf = 1;
            for (int i = (int)factor_count - 1; i >= 1; --i) {
                prod_suf = prod_suf * (seq_t)factors[i];
                sum      = sum      + prod_suf;
            }

            /* If the sum is prime, store the candidate. */
            if (is_prime_big(sum)) {
                seq_append((seq_t)next_candidate);
            }
        }

        ++next_candidate;
    }

    return seq_cache[(size_t)n - 1];
}
