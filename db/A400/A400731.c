/*
 * A400731  Numbers k such that k^2 - 1 is an exponentially odd number
 *          (A268335).
 * https://oeis.org/A400731
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy, new
 * Formula:   not stated
 * Range:     u8 n <= 154, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Search: the n-th integer k such that k^2 - 1 is exponentially odd
   (i.e., every prime in the factorisation of k^2 - 1 has an odd exponent) */
#include <stdbool.h>
#include <stdint.h>
#include <stdlib.h>

/* Prime sieve support ----------------------------------------------------- */
static idx_t prime_limit = 0;          /* largest number up to which primes are generated */
static uint32_t *primes = NULL;        /* array of primes */
static size_t primes_count = 0;        /* number of primes stored */

static void ensure_primes(idx_t limit)
{
    if (limit <= prime_limit) return;
    size_t l = (size_t)limit;

    /* Allocate boolean array for marking composites */
    char *is_comp = (char *)calloc(l + 1, 1);
    if (!is_comp) abort();

    /* Allocate space for primes up to l (worst case l+1 elements) */
    uint32_t *new_primes = (uint32_t *)realloc(primes, (l + 1) * sizeof(uint32_t));
    if (!new_primes) abort();
    primes = new_primes;

    size_t cnt = 0;
    for (size_t p = 2; p <= l; ++p) {
        if (!is_comp[p]) {
            primes[cnt++] = (uint32_t)p;
            if (p * p <= l) {
                for (size_t mult = p * p; mult <= l; mult += p)
                    is_comp[mult] = 1;
            }
        }
    }
    primes_count = cnt;
    prime_limit = l;
    free(is_comp);
}

/* Test whether m has only odd prime exponents ----------------------------- */
static bool is_exponentially_odd(idx_t m)
{
    for (size_t i = 0; i < primes_count && (idx_t)primes[i] * primes[i] <= m; ++i) {
        idx_t p = primes[i];
        if (m % p == 0) {
            size_t exp = 0;
            while (m % p == 0) {
                m /= p;
                ++exp;
            }
            if ((exp & 1) == 0) return false;   /* even exponent found */
        }
    }
    /* Remaining m > 1 has exponent 1 (odd) */
    return true;
}

/* Cache of sequence values ----------------------------------------------- */
static idx_t next_k = 2;          /* next candidate to test */
static seq_t *cache = NULL;       /* cached terms of the sequence */
static size_t cache_len = 0;      /* number of cached terms */
static size_t cache_cap = 0;      /* capacity of cache */

static void ensure_cache_capacity(size_t needed)
{
    if (needed <= cache_cap) return;
    size_t new_cap = cache_cap ? cache_cap * 2 : 8;
    while (new_cap < needed) new_cap <<= 1;
    seq_t *new_arr = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
    if (!new_arr) abort();
    cache = new_arr;
    cache_cap = new_cap;
}

/* Main sequence function --------------------------------------------------- */
seq_t a(idx_t n)
{
    if (n == 0) return 0;            /* undefined for n < 1; guard value */

    while (cache_len < n) {
        /* Ensure we have enough primes to factor next_k^2-1 */
        ensure_primes(next_k);

        idx_t m = next_k * next_k - 1;
        if (is_exponentially_odd(m)) {
            ensure_cache_capacity(cache_len + 1);
            cache[cache_len++] = (seq_t)next_k;
        }
        ++next_k;
    }
    return cache[n - 1];
}
