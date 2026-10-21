/*
 * A277624  Composite numbers which have a dominant prime factor. A prime
 *          factor p of n is dominant if floor(sqrt(p)) > (n/p).
 * https://oeis.org/A277624
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 12, u16 n <= 1128, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   _floor_isqrt_u64, build_primes_upto, is_prime, seq_alloc,
 *            seq_realloc
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computation: search composite numbers for dominant prime factor. */

#include <stdint.h>
#include <stddef.h>
#include <stdbool.h>
#include <stdlib.h>

/* external helpers */
#include "../utils/is_prime.h"                 /* bool is_prime(idx_t n); */
#include "../utils/_floor_isqrt_u64.h"         /* idx_t _floor_isqrt_u64(idx_t n); */
#include "../utils/build_primes_upto.h"        /* void build_primes_upto(idx_t limit, uint64_t **primes_out, size_t *cnt_out); */
#include "../utils/seq_alloc.h"                /* seq_t* seq_alloc(size_t cap); */
#include "../utils/seq_realloc.h"              /* seq_t* seq_realloc(seq_t* old, size_t old_cap, size_t new_cap); */

/* static data for prime list and terms cache */
static uint64_t *prime_list   = NULL;   /* primes up to prime_limit */
static size_t    prime_cnt    = 0;      /* number of primes in prime_list */
static idx_t     prime_limit  = 0;      /* current limit of prime list */

static seq_t  *terms = NULL;           /* discovered sequence terms */
static size_t term_len = 0;             /* number of terms stored */
static size_t term_cap = 0;             /* capacity of terms array */

static uint64_t next_checked = 2;       /* next integer to test */

/* ensure the prime list contains all primes up to at least 'limit' */
static void ensure_prime_limit(idx_t limit)
{
    if (prime_limit >= limit) return;
    uint64_t *new_list = NULL;
    size_t   new_cnt   = 0;
    /* Note: build_primes_upto allocates a new array; we free the old one. */
    build_primes_upto(limit, &new_list, &new_cnt);
    if (prime_list) free(prime_list);
    prime_list  = new_list;
    prime_cnt   = new_cnt;
    prime_limit = limit;
}

/* grow the terms array to at least 'min_cap' capacity */
static void grow_terms(size_t min_cap)
{
    if (term_cap >= min_cap) return;
    size_t new_cap = term_cap ? term_cap * 2 : 16;
    while (new_cap < min_cap) new_cap *= 2;
    terms = seq_realloc(terms, term_cap, new_cap);
    term_cap = new_cap;
}

/* returns true iff n has a dominant prime factor */
static bool has_dominant_prime_factor(uint64_t n)
{
    if (is_prime(n)) return false;           /* must be composite */

    /* ensure prime list covers sqrt(n) for factorization */
    idx_t sqrt_n = _floor_isqrt_u64(n);
    ensure_prime_limit(sqrt_n + 1);

    uint64_t temp    = n;
    uint64_t largest = 0;

    for (size_t i = 0; i < prime_cnt; ++i) {
        uint64_t p = prime_list[i];
        if (p * p > temp) break;
        if (temp % p == 0) {
            largest = p;
            do { temp /= p; } while (temp % p == 0);
        }
    }
    if (temp > 1) largest = temp;           /* remaining prime factor */

    /* dominant condition: floor(sqrt(largest)) > n / largest */
    seq_t p_sqrt = (seq_t)_floor_isqrt_u64(largest);
    seq_t lhs    = p_sqrt * (seq_t)largest;
    seq_t rhs    = (seq_t)n;
    return lhs > rhs;
}

seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0;            /* undefined but safe guard */

    /* initialise terms array on first call */
    if (terms == NULL) {
        term_cap = 32;
        terms = seq_alloc(term_cap);
    }

    /* generate terms until we have at least 'n' of them */
    while (term_len < (size_t)n) {
        uint64_t cur = next_checked++;
        if (has_dominant_prime_factor(cur)) {
            if (term_len == term_cap) grow_terms(term_cap * 2);
            terms[term_len++] = (seq_t)cur;
        }
    }
    return terms[(size_t)n - 1];
}
