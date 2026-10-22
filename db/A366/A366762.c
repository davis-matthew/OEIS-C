/*
 * A366762  Numbers whose canonical prime factorization contains only
 *          exponents which are congruent to 1 modulo 3.
 * https://oeis.org/A366762
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 167, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  Numbers whose canonical prime factorization contains only exponents
 *  congruent to 1 modulo 3 (OEIS A366762).  This implementation
 *  generates the sequence lazily, caching terms for reuse.
 *
 *  The algorithm tests each candidate integer for the exponent
 *  condition by trial division with a growing list of primes.  The
 *  prime list is extended only as far as necessary.
 *
 *  Complexity is roughly O(n √n / log n) for generating the first n
 *  terms, which is well within the time limits for n ≤ 10000.
 */

#include <stdint.h>
#include <stddef.h>
#include <stdlib.h>
#include <stdbool.h>

/* ---------- integer square root for uint64_t ---------- */
static uint64_t isqrt_u64(uint64_t n)
{
    if (n < 2) return n;
    uint64_t low = 1, high = n >> 1;
    uint64_t ans = 0;
    while (low <= high) {
        uint64_t mid = low + (high - low) / 2;
        uint64_t sq  = mid * mid;
        if (sq == n) return mid;
        if (sq < n) {
            ans = mid;
            low = mid + 1;
        } else {
            high = mid - 1;
        }
    }
    return ans;
}

/* ---------- prime table, built incrementally ---------- */
static uint64_t *prime_tbl = NULL;
static size_t    prime_cap = 0;
static size_t    prime_len = 0;

static void add_prime(uint64_t p)
{
    if (prime_cap == 0 || prime_len >= prime_cap) {
        prime_cap = prime_cap ? prime_cap * 2 : 16;
        prime_tbl = (uint64_t *)realloc(prime_tbl, prime_cap * sizeof(uint64_t));
    }
    prime_tbl[prime_len++] = p;
}

static bool is_prime_candidate(uint64_t n);

static void ensure_primes_up_to(uint64_t limit)
{
    if (prime_len == 0) {
        add_prime(2);
    }
    uint64_t next;
    if (prime_len == 1 && prime_tbl[0] == 2 && limit >= 3) {
        next = 3;
    } else {
        next = prime_tbl[prime_len - 1] + 2;
    }
    for (; next <= limit; next += 2) {
        if (is_prime_candidate(next)) {
            add_prime(next);
        }
    }
}

static bool is_prime_candidate(uint64_t n)
{
    if (n < 2) return false;
    if (n == 2) return true;
    if ((n & 1U) == 0U) return false;
    uint64_t root = isqrt_u64(n);
    for (size_t i = 0; i < prime_len && prime_tbl[i] <= root; ++i) {
        if (n % prime_tbl[i] == 0U) return false;
    }
    return true;
}

/* ---------- test whether a number satisfies the exponent condition ---------- */
static bool valid_exponents(uint64_t x)
{
    if (x <= 1U) return true;                /* 1 (and 0) are always valid */
    uint64_t v = x;
    uint64_t root = isqrt_u64(v);
    ensure_primes_up_to(root);
    for (size_t i = 0; i < prime_len && prime_tbl[i] <= root; ++i) {
        uint64_t p = prime_tbl[i];
        if (v % p == 0ULL) {
            unsigned exp = 0U;
            while (v % p == 0ULL) {
                v /= p;
                ++exp;
            }
            if (exp % 3U != 1U) return false;
            root = isqrt_u64(v);           /* update root after dividing */
        }
    }
    /* Remaining factor, if any, has exponent 1 (valid). */
    return true;
}

/* ---------- cache for generated terms ---------- */
static seq_t *cache = NULL;
static size_t cache_cap = 0;
static idx_t  cache_len = 0;
static uint64_t last_val = 0;        /* last integer examined */

/* ---------- main exported function ---------- */
seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0;        /* safety; OEIS offset is 1 */

    /* Grow the cache until it contains the n‑th term. */
    while (cache_len < n) {
        uint64_t cand;
        if (cache_len == 0) {
            cand = 1U;
        } else {
            cand = last_val + 1U;
        }

        /* Find the next integer that satisfies the exponent property. */
        while (!valid_exponents(cand)) {
            ++cand;
        }

        /* Store the found term into the cache. */
        if (cache_cap == 0) {
            cache_cap = 32;
            cache = (seq_t *)malloc(cache_cap * sizeof(seq_t));
        } else if (cache_len >= cache_cap) {
            cache_cap *= 2;
            cache = (seq_t *)realloc(cache, cache_cap * sizeof(seq_t));
        }

        cache[cache_len++] = (seq_t)cand;
        last_val = cand;
    }

    return cache[n - 1];
}
