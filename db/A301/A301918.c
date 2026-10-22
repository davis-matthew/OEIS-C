/*
 * A301918  Primes which divide numbers of the form 3^k+3.
 * https://oeis.org/A301918
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 36, u16 n <= 56, u32 n <= 56, u64 n <= 56, u128 n <= 56,
 *            big any n
 * Helpers:   mod_pow
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* A301918 – primes that divide some number of the form 3^k+3. */
/* The term a(n) is the n‑th prime p for which ∃k≥1 such that p | 3^k+3. */

#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>
#include "../utils/mod_pow.h"

/* ------------------------------------------------------------ */
/*  Cached data: primes discoverd so far and the resulting
 *  sequence entries.
 *  All stored as plain C pointers; realloc() must be cast in C++. */
static size_t cache_len = 0;      /* how many sequence terms stored */
static seq_t *cache = NULL;      /* array of primes fulfilling the condition */
static size_t cache_cap = 0;      /* capacity of cache */

/*  Auxiliary prime list used for trial division */
static uint64_t *prime_arr = NULL;/* primes found during search */
static size_t prime_cnt = 0;      /* how many primes stored */
static size_t prime_cap = 0;      /* capacity of prime array */

/*  Next integer considered for primality */
static uint64_t next_candidate = 2;

/* ---- helper ----------------------------------------------------- */
static bool is_prime_uint64(uint64_t x)
{
    if (x < 2) return false;
    for (size_t i = 0; i < prime_cnt; ++i) {
        uint64_t p = prime_arr[i];
        if (p * p > x) break;
        if (x % p == 0) return false;
    }
    return true;
}

/*  Return true iff the multiplicative order of 3 modulo p is even.
 *  This is equivalent to p dividing some 3^k + 3 (with p ≠ 3). */
static bool has_even_order_of_3(uint64_t p)
{
    uint64_t ord = p - 1;          /* current candidate for the order */
    uint64_t n   = ord;            /* remaining factorised part */

    for (size_t i = 0; i < prime_cnt && prime_arr[i] * prime_arr[i] <= n; ++i) {
        uint64_t q = prime_arr[i];
        if (n % q == 0) {
            while (n % q == 0) n /= q;          /* strip factor q completely */

            /* try to reduce ord by q */
            while (ord % q == 0) {
                seq_t test = mod_pow((seq_t)3, (idx_t)(ord / q), (seq_t)p);
                if (test == (seq_t)1) ord /= q;
                else break;
            }
        }
    }
    if (n > 1) {                             /* remaining large prime factor */
        uint64_t q = n;
        while (ord % q == 0) {
            seq_t test = mod_pow((seq_t)3, (idx_t)(ord / q), (seq_t)p);
            if (test == (seq_t)1) ord /= q;
            else break;
        }
    }
    return (ord & 1ULL) == 0;                /* even if reduced to an even value */
}

/* ---- public function --------------------------------------------- */
seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0;             /* defined only for n≥1 */

    /* initialise prime array on first use */
    if (!prime_cap) {
        prime_cap = 16;
        prime_arr = (uint64_t *)malloc(prime_cap * sizeof(uint64_t));
    }

    /* initialise cache on first use */
    if (!cache_cap) {
        cache_cap = 16;
        cache = (seq_t *)malloc(cache_cap * sizeof(seq_t));
    }

    /* generate terms until we have at least n of them */
    while (cache_len < (size_t)n) {
        uint64_t c = next_candidate;
        bool prime = is_prime_uint64(c);

        if (prime) {
            /* add prime to the helper list */
            if (prime_cnt == prime_cap) {
                prime_cap *= 2;
                prime_arr = (uint64_t *)realloc(prime_arr, prime_cap * sizeof(uint64_t));
            }
            prime_arr[prime_cnt++] = c;

            /* decide whether to keep it in the sequence */
            bool accept = (c == 2ULL) || (c == 3ULL) || has_even_order_of_3(c);
            if (accept) {
                if (cache_len == cache_cap) {
                    cache_cap *= 2;
                    cache = (seq_t *)realloc(cache, cache_cap * sizeof(seq_t));
                }
                cache[cache_len++] = (seq_t)c;
            }
        }

        /* move to next candidate (skip even numbers after 2) */
        next_candidate += (c == 2ULL) ? 1ULL : 2ULL;
    }

    return cache[(size_t)n - 1];
}
