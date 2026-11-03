/*
 * A352804  a(n) = A028876(n)/2; numbers k such that 4*k^2 - 5 is prime.
 * https://oeis.org/A352804
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = nth integer k ≥ 2 such that 4*k*k - 5 is prime
 * Range:     u8 n <= 6, u16 n <= 48, u32 n <= 5927, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Validated: standard, pass, 77 terms through n = 9978 (2026-11-03)
 * Helpers:   is_prime_seq, is_prime_u64, mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* The sequence is the k values themselves, starting from 2. */
#include <stdlib.h>
#include <stdbool.h>
#include <stdint.h>
#include <stddef.h>

#include "../utils/is_prime_seq.h"
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

static seq_t *cache = NULL;          /* array of found k's */
static size_t cache_len   = 0;        /* number stored */
static size_t cache_cap   = 0;        /* allocated capacity */
static idx_t  next_k      = 2;        /* next integer to test */

/* Ensure the cache has enough space. */
static void grow_cache(void) {
    size_t new_cap = (cache_cap ? cache_cap * 2 : 16);
    seq_t *tmp = mem_resize(seq_t, cache, new_cap);
    if (!tmp) abort();
    cache = tmp;
    cache_cap = new_cap;
}

/* Append a new value to the cache. */
static void add_to_cache(idx_t k) {
    if (cache_len == cache_cap) grow_cache();
    cache[cache_len++] = (seq_t)k;
}

/* Check whether 4*k^2 - 5 is prime. */
static bool is_k_prime(idx_t k) {
    /* Compute the value in 128‑bit to avoid overflow. */
    __uint128_t val128 = (__uint128_t)4 * k * k - 5;

    /* Use the appropriate prime test depending on the size of seq_t. */
    if (sizeof(seq_t) <= 8) {
        seq_t v = (seq_t)val128;          /* truncation is safe for the tested range */
        return is_prime_seq(v);
    } else {
        seq_t v = (seq_t)val128;          /* seq_t is at least 128 bits or arbitrary precision */
        return is_prime_big(v);
    }
}

/* Pre‑compute terms up to index n (1‑based). */
static void precompute_until(idx_t n) {
    while (cache_len < n) {
        if (is_k_prime(next_k))
            add_to_cache(next_k);
        ++next_k;
    }
}

/* Public routine.  n is 1‑based; return 0 for n == 0 (out of range). */
seq_t a(idx_t n) {
    if (n == 0) return 0;
    precompute_until(n);
    return cache[(size_t)(n - 1)];
}
