/*
 * A289363  Primes p such that p^2 + p - 1 is composite.
 * https://oeis.org/A289363
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 31, u16 n <= 5483, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A289363: primes p such that p^2 + p - 1 is composite.
   The algorithm repeatedly generates primes p and tests whether
   p^2 + p - 1 has a divisor q <= p.  All found primes are cached
   for O(1) retrieval of any term. */

#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>

/* --------------------------------------------------------------------
   Prime list utilities
   -------------------------------------------------------------------- */
static uint64_t *prime_list = NULL;   /* stored primes as uint64_t */
static size_t    prime_cnt  = 0;      /* number of primes stored      */
static size_t    prime_cap  = 0;      /* allocated capacity of list   */

/* Grow the prime list to at least 'need' elements. */
static void
grow_prime_list(size_t need)
{
    if (prime_cap >= need)
        return;
    size_t new_cap = prime_cap ? prime_cap : 16;
    while (new_cap < need)
        new_cap <<= 1;          /* double until large enough   */
    prime_list = (uint64_t *)realloc(prime_list, new_cap * sizeof(uint64_t));
    if (!prime_list) abort();   /* abort on allocation failure */
    prime_cap  = new_cap;
}

/* Append a new prime to the list. */
static void
add_prime(uint64_t p)
{
    grow_prime_list(prime_cnt + 1);
    prime_list[prime_cnt++] = p;
}

/* Return the first prime ≥ 'start'.  The prime list is updated
   automatically with every new prime found. */
static uint64_t
next_prime_uint64(uint64_t start)
{
    if (start <= 2) {
        if (prime_cnt == 0 || prime_list[prime_cnt - 1] != 2)
            add_prime(2);
        return 2;
    }
    if (start <= 3) {
        if (prime_cnt == 0 || prime_list[prime_cnt - 1] < 3)
            add_prime(3);
        return 3;
    }

    uint64_t cand = (start & 1ULL) ? start : start + 1;   /* make odd */

    for (;;) {
        bool is_prime = true;
        for (size_t i = 0; i < prime_cnt; ++i) {
            uint64_t p = prime_list[i];
            if (p * p > cand)
                break;
            if (cand % p == 0) {
                is_prime = false;
                break;
            }
        }
        if (is_prime) {
            add_prime(cand);
            return cand;
        }
        cand += 2;
    }
}

/* Test whether p^2 + p - 1 is composite.  We test divisibility by
   all primes q <= p using modular arithmetic so no overflow occurs. */
static bool
value_is_composite(uint64_t p)
{
    for (size_t i = 0; i < prime_cnt; ++i) {
        uint64_t q = prime_list[i];
        if (q > p)                        /* q > sqrt(val) reached */
            break;
        /* Compute (p^2 + p - 1) mod q without overflow */
        uint64_t pmq  = p % q;
        uint64_t pm1q = (p + 1) % q;
        uint64_t val_mod = (pmq * pm1q) % q;
        val_mod = (val_mod + q - 1 % q) % q;
        if (val_mod == 0)                 /* divisor found */
            return true;
    }
    return false;          /* no divisor ≤ p: probably prime or >p divisor */
}

/* --------------------------------------------------------------------
   Cache for the sequence values
   -------------------------------------------------------------------- */
static seq_t *cache      = NULL;    /* cached sequence values */
static size_t  cache_cap  = 0;
static size_t  cache_len  = 0;      /* number of terms currently cached */

/* Grow the cache to hold at least 'need' elements. */
static void
grow_cache(size_t need)
{
    if (cache_cap >= need)
        return;
    size_t new_cap = cache_cap ? cache_cap : 16;
    while (new_cap < need)
        new_cap <<= 1;
    cache = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
    if (!cache) abort();             /* abort on allocation failure */
    cache_cap = new_cap;
}

/* --------------------------------------------------------------
   The required sequence function
   -------------------------------------------------------------- */
seq_t a(idx_t n)
{
    if (n == 0)          /* safeguard: offset is 1 */
        return (seq_t)0;

    /* Return cached value if already computed. */
    if ((size_t)n <= cache_len)
        return cache[(size_t)n - 1];

    /* Extend cache until it contains the nth term. */
    while (cache_len < (size_t)n) {
        /* Get next prime after the last added prime. */
        uint64_t p;
        if (prime_cnt == 0)
            p = next_prime_uint64(2);
        else
            p = next_prime_uint64(prime_list[prime_cnt - 1] + 1);

        /* Test p^2 + p - 1 for compositeness. */
        if (value_is_composite(p)) {
            grow_cache(cache_len + 1);
            cache[cache_len++] = (seq_t)p;
        }
        /* The prime p has already been added to prime_list
           by next_prime_uint64, ensuring future tests have it. */
    }

    return cache[(size_t)n - 1];
}
