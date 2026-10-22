/*
 * A344296  Numbers with at least as many prime factors (counted with
 *          multiplicity) as half their sum of prime indices.
 * https://oeis.org/A344296
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 50, u16 n <= 60, u32 n <= 60, u64 n <= 60, u128 n <= 60,
 *            big any n
 * Helpers:   _floor_isqrt_u64, is_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  OEIS A344296
 *  Numbers with at least as many prime factors (counted with multiplicity)
 *  as half their sum of prime indices.
 *
 *  Implementation: trial division of consecutive integers, testing the inequality
 *  2 * Ω(n) ≥ Σ ( prime_index of each prime factor of n ).  Results are
 *  cached in the order they are found, so that calls to a(n) in any order
 *  are answered in amortised O(n) time for n up to about 10 000.
 *
 *  The code is idiomatic C11/C++17, uses only integer arithmetic, and
 *  relies on helper functions provided in this project.
 */

#include <stdint.h>
#include <stddef.h>
#include <stdbool.h>
#include <stdlib.h>

#include "../utils/is_prime.h"           /* bool is_prime(idx_t)    */
#include "../utils/_floor_isqrt_u64.h"   /* idx_t _floor_isqrt_u64(idx_t) */

static seq_t   *term_cache = NULL;    /* cached sequence values        */
static size_t   cache_cnt  = 0;       /* number of cached terms         */
static size_t   cache_cap  = 0;       /* capacity of term_cache         */

static uint64_t *prime_list = NULL;   /* small primes, 1‑based indices  */
static size_t   prime_cnt  = 0;       /* number of primes               */
static size_t   prime_cap  = 0;       /* capacity of prime_list         */

static uint64_t next_test = 1;        /* next integer to be examined    */

/* --------------------------------------------------------------------- */
/*          Prime list handling                                            */
/* --------------------------------------------------------------------- */

static void ensure_prime_capacity(size_t min_cap)
{
    if (prime_cap >= min_cap)
        return;
    size_t new_cap = prime_cap ? prime_cap * 2 : 16;
    while (new_cap < min_cap)
        new_cap *= 2;
    uint64_t *tmp = (uint64_t *)realloc(prime_list, new_cap * sizeof(uint64_t));
    if (!tmp) abort();          /* memory failure */
    prime_list = tmp;
    prime_cap  = new_cap;
}

static void add_prime(uint64_t p)
{
    ensure_prime_capacity(prime_cnt + 1);
    prime_list[prime_cnt++] = p;
}

static void ensure_prime_limit(uint64_t limit)
{
    if (prime_cnt && prime_list[prime_cnt - 1] >= limit)
        return;
    uint64_t cand = prime_cnt ? prime_list[prime_cnt - 1] + 1 : 2;
    while (prime_cnt == 0 || prime_list[prime_cnt - 1] < limit) {
        if (is_prime((idx_t)cand))
            add_prime(cand);
        ++cand;
    }
}

/* binary search assuming p is in prime_list */
static size_t binary_search_prime_index(uint64_t p)
{
    size_t lo = 0, hi = prime_cnt;        /* hi is exclusive */
    while (lo < hi) {
        size_t mid = (lo + hi) >> 1;
        if (prime_list[mid] == p)
            return mid + 1;                /* 1‑based index */
        if (prime_list[mid] < p)
            lo = mid + 1;
        else
            hi = mid;
    }
    return 0;    /* should never happen */
}

static size_t find_prime_index(uint64_t p)
{
    ensure_prime_limit(p);
    return binary_search_prime_index(p);
}

/* --------------------------------------------------------------------- */
/*          Sequence generation                                            */
/* --------------------------------------------------------------------- */

static void ensure_terms(idx_t needed)
{
    while (cache_cnt < (size_t)needed) {
        uint64_t m = next_test++;

        /* m == 1 always satisfies the condition */
        if (m == 1) {
            if (cache_cnt == cache_cap) {
                size_t new_cap = cache_cap ? cache_cap * 2 : 8;
                seq_t *tmp = (seq_t *)realloc(term_cache, new_cap * sizeof(seq_t));
                if (!tmp) abort();
                term_cache = tmp;
                cache_cap  = new_cap;
            }
            term_cache[cache_cnt++] = (seq_t)m;
            continue;
        }

        /* Prepare primes up to sqrt(m) */
        uint64_t sqrt_m = _floor_isqrt_u64((idx_t)m) + 1;
        ensure_prime_limit(sqrt_m);

        unsigned long long omega = 0;
        unsigned long long sum_idx = 0;
        uint64_t temp = m;

        /* Trial division by known primes */
        for (size_t i = 0; i < prime_cnt; ++i) {
            uint64_t p = prime_list[i];
            if (p > sqrt_m || p * p > temp)
                break;                    /* p*p may overflow; use sqrt_m guard */
            if (temp % p != 0)
                continue;
            do {
                temp /= p;
                ++omega;
                sum_idx += (i + 1);        /* 1‑based prime index */
            } while (temp % p == 0);
            sqrt_m = _floor_isqrt_u64((idx_t)temp) + 1;
        }

        /* Remaining factor, if any, is a prime > sqrt(original m) */
        if (temp > 1) {
            ++omega;
            sum_idx += find_prime_index(temp);
        }

        /* Test the defining inequality */
        if ((omega << 1) >= sum_idx) {
            if (cache_cnt == cache_cap) {
                size_t new_cap = cache_cap ? cache_cap * 2 : 8;
                seq_t *tmp = (seq_t *)realloc(term_cache, new_cap * sizeof(seq_t));
                if (!tmp) abort();
                term_cache = tmp;
                cache_cap  = new_cap;
            }
            term_cache[cache_cnt++] = (seq_t)m;
        }
    }
}

/* --------------------------------------------------------------------- */
/*          Public API                                                    */
/* --------------------------------------------------------------------- */

seq_t a(idx_t n)
{
    if (n == 0)  return 0;              /* value outside the sequence */
    ensure_terms(n);
    return term_cache[n - 1];
}
