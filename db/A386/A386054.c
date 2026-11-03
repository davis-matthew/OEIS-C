/*
 * A386054  Primes having only {0, 2, 7, 9} as digits.
 * https://oeis.org/A386054
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base, easy
 * Formula:   not stated
 * Range:     u8 n <= 7, u16 n <= 68, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Validated: standard, pass, 63 terms through n = 8581 (2026-11-03)
 * Helpers:   is_prime_u64, mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Description:  Primes whose decimal digits are only 0,2,7,9
   The function computes the n-th such prime (1‑based indexing).  */

/* required utility headers */
#include <stddef.h>
#include <stdint.h>
#include <stdbool.h>
#include <stdlib.h>
#include "../utils/mem.h"
#include "../utils/is_prime_u64.h"

/* Append a value to the cached array, expanding it if necessary.  `p` is a
   pointer to the array pointer. */
static void append_candidate(seq_t **p, size_t *len, size_t *cap, seq_t val)
{
    if (*len == *cap) {
        size_t newcap = (*cap == 0) ? 16 : (*cap << 1);
        seq_t *tmp = mem_resize(seq_t, *p, newcap);
        if (!tmp) {
            exit(EXIT_FAILURE);
        }
        *p = tmp;
        *cap = newcap;
    }
    (*p)[*len] = val;
    (*len)++;
}

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

/* static data for computation and caching */
static seq_t *cache   = NULL;   /* array of found primes           */
static size_t cache_len = 0;    /* number of primes stored         */
static size_t cache_cap = 0;    /* current allocation capacity     */
static idx_t  next_len  = 1;    /* next digit length to generate   */

/* digit sets used in generation */
static const uint8_t first_digits[3] = {2, 7, 9};   /* first digit cannot be 0 */
static const uint8_t other_digits[4] = {0, 2, 7, 9};

/* recursive helper to generate numbers of a fixed length */
static void generate_rec(idx_t pos, seq_t val, idx_t len)
{
    if (pos == len) {
        if (is_prime_big(val))
            append_candidate(&cache, &cache_len, &cache_cap, val);
        return;
    }
    for (int i = 0; i < 4; ++i) {
        seq_t next = val * 10 + other_digits[i];
        generate_rec(pos + 1, next, len);
    }
}

/* generate all primes of exactly 'len' decimal digits consisting of
   allowed digits and store them in the cache */
static void generate_len(idx_t len)
{
    if (len == 1) {
        for (int i = 0; i < 3; ++i) {
            seq_t v = first_digits[i];
            if (is_prime_big(v))
                append_candidate(&cache, &cache_len, &cache_cap, v);
        }
        return;
    }
    for (int i = 0; i < 3; ++i) {
        seq_t start = first_digits[i];
        generate_rec(1, start, len);
    }
}

/* public sequence function */
seq_t a(idx_t n)
{
    if (n == 0)
        return (seq_t)0;   /* undefined for n<1, return 0 gracefully */

    /* extend cache until it contains at least n primes */
    while ((idx_t)cache_len < n) {
        generate_len(next_len);
        ++next_len;
    }
    return cache[n - 1];
}
