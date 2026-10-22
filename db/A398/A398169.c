/*
 * A398169  Squarefree numbers k such that k+1 is an exponentially odd number.
 * https://oeis.org/A398169
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 107, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* A398169: squarefree k such that k+1 has all prime exponents odd.
   Computed by linear search with a small prime sieve. */

#include <stdbool.h>
#include <stdint.h>
#include <stdlib.h>
#include <stddef.h>

/* ---------- integer square root (floor) -------------------------------- */
static uint64_t isqrt_u64(uint64_t n)
{
    uint64_t r = 0;
    uint64_t bit = 1ULL << 62;              /* 2^(2k) <= n */
    while (bit > n) bit >>= 2;
    while (bit) {
        if (n >= r + bit) {
            n -= r + bit;
            r = (r >> 1) + bit;
        } else {
            r >>= 1;
        }
        bit >>= 2;
    }
    return r;
}

/* ---------- prime sieve ---------------------------------------------- */
static uint64_t *primes      = NULL;
static size_t   primes_cnt   = 0;
static uint64_t primes_limit = 0;           /* highest prime sieved so far */

/* Sieve primes up to 'limit' (inclusive). */
static void build_primes_up_to(uint64_t limit)
{
    if (limit <= primes_limit) return;

    size_t size = limit + 1;
    bool *comp = (bool *)malloc(size * sizeof(bool));
    if (!comp) exit(1);
    for (size_t i = 0; i < size; ++i) comp[i] = false;
    comp[0] = comp[1] = true;

    for (uint64_t i = 2; i * i <= limit; ++i) {
        if (!comp[i]) {
            for (uint64_t j = i * i; j <= limit; j += i)
                comp[j] = true;
        }
    }

    size_t count = 0;
    for (size_t i = 2; i <= limit; ++i)
        if (!comp[i]) ++count;

    uint64_t *new_primes = (uint64_t *)realloc(primes,
                                               count * sizeof(uint64_t));
    if (!new_primes) exit(1);
    primes = new_primes;

    size_t idx = 0;
    for (size_t i = 2; i <= limit; ++i)
        if (!comp[i]) primes[idx++] = i;

    primes_cnt   = count;
    primes_limit = limit;
    free(comp);
}

/* Ensure primes are sieved up to sqrt(n)+1. */
static void ensure_primes_for(uint64_t n)
{
    uint64_t lim = isqrt_u64(n) + 1;
    build_primes_up_to(lim);
}

/* ---------- helper predicates ---------------------------------------- */

/* True if n is squarefree. */
static bool is_squarefree_uint64(uint64_t n)
{
    if (n <= 1) return true;
    if (primes_cnt == 0) ensure_primes_for(n);
    for (size_t i = 0; i < primes_cnt; ++i) {
        uint64_t p = primes[i];
        if (p * p > n) break;
        if (n % p == 0) {
            n /= p;
            if (n % p == 0) return false;
        }
    }
    return true;
}

/* True if all prime exponents in n are odd. */
static bool all_exponents_odd(uint64_t n)
{
    if (n <= 1) return true;               /* 1 has no prime factors */
    if (primes_cnt == 0) ensure_primes_for(n);
    for (size_t i = 0; i < primes_cnt; ++i) {
        uint64_t p = primes[i];
        if (p * p > n) break;
        if (n % p == 0) {
            int exp = 0;
            while (n % p == 0) {
                n /= p;
                ++exp;
            }
            if ((exp % 2) == 0) return false;
        }
    }
    return true;                           /* remaining factor has exp 1 */
}

/* ---------- cache for computed terms --------------------------------- */
static seq_t *cache   = NULL;
static size_t cache_len = 0;
static size_t cache_cap = 0;

static void push_term(seq_t val)
{
    if (cache_cap == 0) {
        cache_cap = 64;
        cache = (seq_t *)malloc(cache_cap * sizeof(seq_t));
        if (!cache) exit(1);
    } else if (cache_len == cache_cap) {
        cache_cap *= 2;
        seq_t *tmp = (seq_t *)realloc(cache, cache_cap * sizeof(seq_t));
        if (!tmp) exit(1);
        cache = tmp;
    }
    cache[cache_len++] = val;
}

/* Generate sequence terms up to index n (1‑based). */
static void generate_up_to(idx_t n)
{
    if (n <= cache_len) return;

    uint64_t next = 1;
    if (cache_len > 0)
        next = (uint64_t)cache[cache_len - 1] + 1;

    while (cache_len < n) {
        ensure_primes_for(next);
        ensure_primes_for(next + 1);

        if (is_squarefree_uint64(next) && all_exponents_odd(next + 1))
            push_term((seq_t)next);
        ++next;
    }
}

/* ---------- public interface ----------------------------------------- */
seq_t a(idx_t n)
{
    if (n == 0) return 0;          /* undefined for n < 1 */
    generate_up_to(n);
    return cache[n - 1];           /* 0‑based array */
}
