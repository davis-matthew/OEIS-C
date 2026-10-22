/*
 * A375710  Numbers k such that A013929(k+1) - A013929(k) = 2. In other words,
 *          the k-th nonsquarefree number is 2 less than the next
 *          nonsquarefree number.
 * https://oeis.org/A375710
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 57, u16 n <= 57, u32 n <= 57, u64 n <= 57, u128 n <= 57,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computes A375710:  the indices k such that the k‑th and (k+1)‑st
 * nonsquarefree numbers differ by exactly 2.  The function works
 * for any idx_t and seq_t supplied by the build and is memoised so
 * that calls in arbitrary order remain efficient. */

#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>

/* ------------------------------------------------------------ */
/*  Integer square root (floor) for 64‑bit values                */
static uint64_t isqrt_uint64(uint64_t x)
{
    uint64_t r = 0, bit = 1ULL << 32;          /* 2^32 is > sqrt(2^64)   */
    while (bit > x) bit >>= 2;
    while (bit) {
        if (x >= r + bit) {
            x -= r + bit;
            r = (r >> 1) + bit;
        } else {
            r >>= 1;
        }
        bit >>= 2;
    }
    return r;
}

/* ------------------------------------------------------------ */
/*  Prime sieve helpers (incremental)                           */
static uint64_t *primes = NULL;          /* array of primes, unsorted   */
static size_t primes_len = 0;
static size_t primes_cap = 0;
static uint64_t next_prime_candidate = 2;  /* next integer to test        */

static void add_prime(uint64_t p)
{
    if (primes_len == primes_cap) {
        size_t new_cap = primes_cap ? primes_cap * 2 : 4;
        uint64_t *tmp = (uint64_t *)realloc(primes, new_cap * sizeof(uint64_t));
        primes = tmp;
        primes_cap = new_cap;
    }
    primes[primes_len++] = p;
}

static bool is_candidate_prime(uint64_t cand)
{
    if (cand < 2) return false;
    for (size_t i = 0; i < primes_len; ++i) {
        uint64_t p = primes[i];
        if ((unsigned __int128)p * p > cand) break;
        if (cand % p == 0) return false;
    }
    return true;
}

static void ensure_primes_up_to(uint64_t limit)
{
    for (uint64_t c = next_prime_candidate; c <= limit; ++c) {
        if (is_candidate_prime(c)) add_prime(c);
    }
    next_prime_candidate = limit + 1;
}

/* ------------------------------------------------------------ */
/*  Nonsquarefree number generator                               */
static uint64_t cur = 2;            /* current integer being tested   */
static uint64_t nsf_counter = 0;    /* count of nonsquarefree numbers seen */
static uint64_t prev_value = 0;     /* previous nonsquarefree value   */
static uint64_t prev_index = 0;     /* index of previous nonsquarefree */

/*  Returns the next nonsquarefree integer, advancing state.     */
static uint64_t next_non_squarefree(void)
{
    while (1) {
        uint64_t sqrt_cur = isqrt_uint64(cur);
        ensure_primes_up_to(sqrt_cur);
        bool nsf = false;
        for (size_t i = 0; i < primes_len; ++i) {
            uint64_t p = primes[i];
            unsigned __int128 sq = (unsigned __int128)p * p;
            if (sq > cur) break;
            if (cur % (uint64_t)sq == 0) {
                nsf = true;
                break;
            }
        }
        if (nsf) return cur;
        ++cur;
    }
}

/* ------------------------------------------------------------ */
/*  Sequence cache:  cache entries are seq_t holding the index
 *  value (k).  The cache holds all values computed up to the
 *  maximum n requested so far. */
static seq_t *cache = NULL;
static size_t cache_len = 0;
static size_t cache_cap = 0;

/*  Public function implementing A375710                   */
seq_t a(idx_t n)
{
    if (n == 0) return 0;                      /* offset is 1 */
    if ((size_t)n <= cache_len) return cache[n - 1];

    while (cache_len < (size_t)n) {
        uint64_t curr_val = next_non_squarefree();  /* value of this nsf */
        ++nsf_counter;                            /* its index  (1‑based) */

        if (nsf_counter >= 2) {
            uint64_t diff = curr_val - prev_value;
            if (diff == 2) {                     /* record previous nsf index */
                if (cache_len == cache_cap) {
                    size_t new_cap = cache_cap ? cache_cap * 2 : 8;
                    seq_t *tmp = (seq_t *)realloc(cache,
                                                   new_cap * sizeof(seq_t));
                    cache = tmp;
                    cache_cap = new_cap;
                }
                cache[cache_len++] = (seq_t)prev_index;
            }
        }

        prev_value = curr_val;
        prev_index = nsf_counter;
        ++cur; /* skip past the current number for next search */
    }

    return cache[n - 1];
}
