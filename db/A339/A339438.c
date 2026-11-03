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
 * Range:     u8 n <= 43, u16 n <= 5066, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Validated: standard, pass, 82 terms through n = 7572 (2026-11-03)
 * Helpers:   is_prime, is_prime_u64, isqrt, mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stddef.h>
#include <stdbool.h>
#include <stdlib.h>

/* Utility headers supplied by the build framework. */
#include "../utils/is_prime.h"
#include "../utils/isqrt.h"
#include "../utils/mem.h"
#include "../utils/is_prime_u64.h"

/* Miller–Rabin primality test for seq_t (supports 8/16/32/64/128/ arbitrary-precision) */
/* n fits in uint64_t (n >= 0 here) */
static bool is_prime_big_fits_u64(seq_t v)
{
    if (sizeof(seq_t) <= 8) return true;
    return !((seq_t)UINT64_MAX < v);
}

static bool is_prime_big(seq_t n) {
    if (n < 2) return false;
    /* exact in 64 bits when n fits: the seq_t products below overflow at
       u64/i64 once n passes 2^32, and are slow under big */
    if (is_prime_big_fits_u64(n)) return is_prime_u64((uint64_t)n);
    if (n == 2 || n == 3) return true;
    if ((n & 1) == 0) return false;

    /* write n-1 as d * 2^s with d odd */
    seq_t d = n - 1;
    unsigned s = 0;
    while ((d & 1) == 0) {
        d >>= 1;
        s++;
    }

    /* deterministic bases covering 128‑bit values */
    static const int base_list[] = {2,3,5,7,11,13,17,19,23,29,31,37};
    const int num_bases = sizeof(base_list)/sizeof(base_list[0]);

    for (int i = 0; i < num_bases; i++) {
        seq_t a = (seq_t)base_list[i];
        if (a >= n) continue;

        /* compute a^d mod n */
        seq_t x = 1;
        seq_t pow = a % n;
        seq_t exp = d;
        while (exp > 0) {
            if (exp & 1) x = (x * pow) % n;
            pow = (pow * pow) % n;
            exp >>= 1;
        }

        if (x == 1 || x == n-1) continue;

        bool cont = false;
        for (unsigned r = 1; r < s; r++) {
            x = (x * x) % n;
            if (x == n-1) { cont = true; break; }
        }
        if (!cont) return false;
    }
    return true;
}

#if defined(__cplusplus)
/* C++ casts for realloc. */
#define RELOC_PTR(ptr, type, new_cap) (ptr) = mem_resize(type, (ptr), (new_cap))
#else
#define RELOC_PTR(ptr, type, new_cap) (ptr) = (mem_resize(type, (ptr), (new_cap)))
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
        uint64_t limit = isqrt((idx_t)next_candidate);
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
