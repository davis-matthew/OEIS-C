/*
 * A347344  Positive integers k such that k with the first (most significant)
 *          digit repeated is prime.
 * https://oeis.org/A347344
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) = the n‑th positive integer k such that D(k) = k with its
 *            most significant digit repeated is prime, where D(k) = d·10^m +
 *            k, d = first digit of k and m = number of digits of k
 * Range:     u8 n <= 5, u16 n <= 847, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Validated: standard, pass, 71 terms through n = 9364 (2026-11-03)
 * Helpers:   is_prime_u64, mem, num_digits_idx
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include "../utils/num_digits_idx.h"
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

/* Compute 10^e for a non‑negative integer exponent e */
static seq_t pow10(idx_t e) {
    seq_t res = 1;
    while (e) {
        res *= 10;
        e--;
    }
    return res;
}

/* Helper to compute the most significant digit of a uint64_t value. */
static unsigned leading_digit(uint64_t x)
{
    while (x >= 10) x /= 10;
    return (unsigned)x;
}

/* Cached results of the sequence. */
static seq_t *cache      = NULL;
static size_t cache_cap  = 0;
static size_t cache_len  = 0;

/* The largest k that has been tested. */
static uint64_t tested_k = 1;

/* Grow the cache buffer if necessary. */
static void ensure_cache_cap(size_t need)
{
    if (cache_cap >= need) return;
    size_t new_cap = cache_cap ? cache_cap * 2 : 64;
    if (new_cap < need) new_cap = need;
    seq_t *new_arr = mem_new(seq_t, new_cap);
    if (!new_arr) exit(1);
    if (cache) {
        mem_copy(new_arr, cache, cache_len * sizeof(seq_t));
        mem_free(cache);
    }
    cache     = new_arr;
    cache_cap = new_cap;
}

/* Append a new element to the cache. */
static void cache_append(seq_t v)
{
    ensure_cache_cap(cache_len + 1);
    cache[cache_len++] = v;
}

/* Main sequence function. */
seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0; /* offset 1; n>=1 in normal use */

    /* Generate terms until we have at least n of them. */
    while (cache_len < (size_t)n) {
        uint64_t k = tested_k++;

        unsigned d = leading_digit(k);
        int dlen   = num_digits_idx((idx_t)k);          /* number of digits of k */
        seq_t D    = (seq_t)d * pow10((idx_t)dlen) + (seq_t)k;

        if (is_prime_big(D)) {
            cache_append((seq_t)k);
        }
    }
    return cache[(size_t)(n - 1)];
}
