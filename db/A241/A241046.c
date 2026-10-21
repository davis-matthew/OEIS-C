/*
 * A241046  Primes having primitive roots 2, 3, 5, 7, and 11.
 * https://oeis.org/A241046
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 1, u16 n <= 100, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   seq_alloc, seq_realloc
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence: primes p such that 2, 3, 5, 7, and 11 are primitive roots modulo p */

#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>
#include "../utils/seq_alloc.h"
#include "../utils/seq_realloc.h"

/* Dynamic storage for primes discovered so far */
static uint64_t *prime_list  = NULL;
static size_t   prime_cnt    = 0;
static size_t   prime_cap    = 0;

/* Dynamic storage for the sequence values */
static seq_t   *results      = NULL;
static size_t   res_cnt      = 0;
static size_t   res_cap      = 0;

/* Next candidate for primality testing */
static uint64_t next_candidate = 2;

/* Helper: enlarge prime list when needed (C++ requires explicit cast) */
static void ensure_prime_cap(size_t needed) {
    if (prime_cap < needed) {
        size_t new_cap = prime_cap ? prime_cap << 1 : 64;
        while (new_cap < needed) new_cap <<= 1;
        uint64_t *newp = (uint64_t*)realloc(prime_list, new_cap * sizeof(uint64_t));
        if (!newp) abort();
        prime_list = newp;
        prime_cap  = new_cap;
    }
}

/* Helper: enlarge results array when needed (uses lib helper to construct objects) */
static void ensure_res_cap(size_t needed) {
    if (res_cap < needed) {
        size_t new_cap = res_cap ? res_cap << 1 : 64;
        while (new_cap < needed) new_cap <<= 1;
        if (results == NULL) {
            results = seq_alloc(new_cap);
        } else {
            results = seq_realloc(results, res_cap, new_cap);
        }
        res_cap = new_cap;
    }
}

/* Modular exponentiation for uint64_t arguments */
static uint64_t pow_mod(uint64_t base, uint64_t exp, uint64_t mod) {
    uint64_t res = 1;
    base %= mod;
    while (exp) {
        if (exp & 1) {
            res = (unsigned __int128)res * base % mod;
        }
        exp >>= 1;
        if (exp) {
            base = (unsigned __int128)base * base % mod;
        }
    }
    return res;
}

/* Fast trial division primality test using known primes */
static bool is_prime(uint64_t n) {
    if (n < 2) return false;
    for (size_t i = 0; i < prime_cnt; ++i) {
        uint64_t p = prime_list[i];
        if ((unsigned __int128)p * p > n) break;
        if (n % p == 0) return false;
    }
    return true;
}

/* Test whether all bases 2,3,5,7,11 are primitive roots modulo p */
static bool good_prime(uint64_t p) {
    if (p <= 11) return false;                 /* bases coincide with p */

    static const uint64_t bases[5] = {2, 3, 5, 7, 11};
    uint64_t m   = p - 1;
    uint64_t rem = m;

    /* Factor m using current prime list */
    for (size_t i = 0; i < prime_cnt && (unsigned __int128)prime_list[i] * prime_list[i] <= rem; ++i) {
        uint64_t q = prime_list[i];
        if (rem % q == 0) {
            /* Check each base for this prime divisor */
            for (int j = 0; j < 5; ++j) {
                if (pow_mod(bases[j], m / q, p) == 1) return false;
            }
            while (rem % q == 0) rem /= q;
        }
    }
    if (rem > 1) {   /* remaining prime factor */
        for (int j = 0; j < 5; ++j) {
            if (pow_mod(bases[j], m / rem, p) == 1) return false;
        }
    }
    return true;
}

/* External linkage function requested by the framework */
seq_t a(idx_t n) {
    if (n == 0) return (seq_t)0;           /* undefined for n=0, return 0 */

    size_t target = (size_t)n;
    while (res_cnt < target) {
        /* Search for the next prime */
        while (true) {
            if (is_prime(next_candidate)) {
                /* Record new prime */
                ensure_prime_cap(prime_cnt + 1);
                prime_list[prime_cnt++] = next_candidate;

                /* If it satisfies the primitive‑root condition, store it */
                if (good_prime(next_candidate)) {
                    ensure_res_cap(res_cnt + 1);
                    results[res_cnt++] = (seq_t)next_candidate;
                }
                ++next_candidate;
                break;   /* move on to looking for the next prime */
            }
            ++next_candidate;
        }
    }
    return results[n - 1];
}
