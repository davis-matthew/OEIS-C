/*
 * A343340  Numbers with a prime index dividing all the other prime indices,
 *          but with no prime index divisible by all the other prime indices.
 * https://oeis.org/A343340
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 25, u16 n <= 53, u32 n <= 53, u64 n <= 53, u128 n <= 53,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* A343340: numbers n > 1 whose smallest prime index divides all the other
 * prime indices, but whose greatest prime index is not divisible by all
 * other prime indices.  The function computes the sequence by testing each
 * integer in ascending order until the requested index is reached. */

#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>

/* ---------- Utility: integer sqrt for uint64_t ---------- */
static uint64_t isqrt_u64(uint64_t n)
{
    if (n < 2)
        return n;
    uint64_t x = n;
    uint64_t y = (x + 1) >> 1;
    while (y < x) {
        x = y;
        y = (x + n / x) >> 1;
    }
    return x;
}

/* ---------- Prime list maintenance ---------- */
static uint64_t *prime_list = NULL;   /* array of primes */
static size_t     prime_cap  = 0;    /* capacity of array   */
static size_t     prime_len  = 0;    /* number of primes    */
static uint64_t   prime_next = 2;    /* next integer to test for primality */

/* Ensure prime_list contains all primes <= limit. */
static void ensure_primes_up_to(uint64_t limit)
{
    if (prime_next > limit)
        return;

    if (prime_cap == 0) {
        prime_cap = 16;
        prime_list = (uint64_t*)realloc(NULL, prime_cap * sizeof(uint64_t));
        if (!prime_list) exit(EXIT_FAILURE);
    }

    for (uint64_t x = prime_next; x <= limit; ++x) {
        bool is_p = true;
        for (size_t i = 0; i < prime_len; ++i) {
            uint64_t p = prime_list[i];
            if ((uint64_t)p * (uint64_t)p > x)
                break;
            if (x % p == 0) {
                is_p = false;
                break;
            }
        }
        if (is_p) {
            if (prime_len == prime_cap) {
                prime_cap *= 2;
                prime_list = (uint64_t*)realloc(prime_list,
                                                prime_cap * sizeof(uint64_t));
                if (!prime_list) exit(EXIT_FAILURE);
            }
            prime_list[prime_len++] = x;
        }
    }
    prime_next = limit + 1;
}

/* Return 1‑based index of prime p; p must be prime. */
static uint64_t prime_index(uint64_t p)
{
    if (p < 2)
        return 0;
    if (prime_len == 0 || prime_list[prime_len - 1] < p)
        ensure_primes_up_to(p);
    size_t lo = 0, hi = prime_len;
    while (lo < hi) {
        size_t mid = (lo + hi) >> 1;
        if (prime_list[mid] < p)
            lo = mid + 1;
        else
            hi = mid;
    }
    return (lo < prime_len && prime_list[lo] == p) ? (lo + 1) : 0;
}

/* ---------- Factorisation ---------- */
/* Fill distinct prime factors of n into array 'factors', up to 'maxf' slots.
   Return the number of distinct prime factors. */
static size_t factor_distinct(uint64_t n, uint64_t *factors, size_t maxf)
{
    size_t cnt = 0;
    uint64_t temp = n;
    uint64_t limit = isqrt_u64(temp);
    ensure_primes_up_to(limit);
    for (size_t i = 0; i < prime_len && prime_list[i] <= limit; ++i) {
        uint64_t p = prime_list[i];
        if (temp % p == 0) {
            if (cnt < maxf)
                factors[cnt++] = p;
            while (temp % p == 0)
                temp /= p;
            limit = isqrt_u64(temp);
        }
    }
    if (temp > 1) {
        if (cnt < maxf)
            factors[cnt++] = temp;
    }
    return cnt;
}

/* ---------- Good‑number test ---------- */
static bool good_number(uint64_t m)
{
    if (m <= 1)
        return false;

    uint64_t primes[64];
    size_t nf = factor_distinct(m, primes, 64);
    if (nf <= 1)
        return false;                /* need at least two distinct primes */

    uint64_t idxs[64];
    for (size_t i = 0; i < nf; ++i)
        idxs[i] = prime_index(primes[i]);

    /* condition 1: smallest index divides all others */
    uint64_t min_idx = idxs[0];
    for (size_t i = 1; i < nf; ++i)
        if (idxs[i] < min_idx)
            min_idx = idxs[i];
    for (size_t i = 0; i < nf; ++i)
        if (idxs[i] % min_idx != 0)
            return false;

    /* condition 2: no index divisible by all others */
    for (size_t i = 0; i < nf; ++i) {
        bool all_div = true;
        for (size_t j = 0; j < nf; ++j) {
            if (i == j)
                continue;
            if (idxs[i] % idxs[j] != 0) {
                all_div = false;
                break;
            }
        }
        if (all_div)
            return false;
    }
    return true;
}

/* ---------- Sequence storage (memoisation) ---------- */
static seq_t *seq_cache = NULL;   /* cached sequence values */
static size_t cache_cap = 0;      /* capacity of seq_cache  */
static size_t cache_len = 0;      /* number of cached values */
static uint64_t next_candidate = 2; /* next integer to test */

/* Ensure seq_cache can hold at least 'needed' elements. */
static void ensure_cache(size_t needed)
{
    if (needed <= cache_cap)
        return;
    size_t new_cap = cache_cap == 0 ? 16 : cache_cap * 2;
    while (new_cap < needed)
        new_cap *= 2;
    seq_cache = (seq_t*)realloc(seq_cache, new_cap * sizeof(seq_t));
    if (!seq_cache)
        exit(EXIT_FAILURE);
    cache_cap = new_cap;
}

/* ---------- Public function ---------- */
seq_t a(idx_t n)
{
    if (n == 0)
        return (seq_t)0;           /* undefined for offset 1, but safe */

    /* Fill cache up to index n */
    while (cache_len < (size_t)n) {
        if (good_number(next_candidate)) {
            ensure_cache(cache_len + 1);
            seq_cache[cache_len++] = (seq_t)next_candidate;
        }
        ++next_candidate;
    }
    return seq_cache[n - 1];
}
