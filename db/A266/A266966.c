/*
 * A266966  Primes of the form x^2 + y^2 where x^2 + x*y + y^2 is also prime
 *          with positive x and y.
 * https://oeis.org/A266966
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 6, u16 n <= 53, u32 n <= 53, u64 n <= 53, u128 n <= 53,
 *            big any n
 * Validated: partial, pass, 53 terms through n = 53 (2026-11-03)
 * Helpers:   ipow_i32, is_prime_u64, mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* A266966: primes p such that p = x^2 + y^2 (x>0,y>0)
   and q = x^2 + x*y + y^2 is also prime.
   The function finds terms in increasing order, caching
   results for repeated calls. */

#include <stdbool.h>
#include <stdlib.h>
#include <stdint.h>
#include "../utils/ipow_i32.h"
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

/* Floor square root by binary search */
static inline seq_t floor_sqrt(seq_t x)
{
    seq_t lo = 0, hi = 1;
    while (ipow_i32(hi, 2) <= x)
        hi <<= 1;
    while (lo < hi)
    {
        seq_t mid = (lo + hi) >> 1;
        seq_t mid2 = mid * mid;
        if (mid2 <= x)
            lo = mid + 1;
        else
            hi = mid;
    }
    return lo - 1;
}

/* cached sequence – one‑based externally */
static seq_t *cache = NULL;
static size_t cache_len = 0;
static size_t cache_cap = 0;
static idx_t tested = 1;   /* last integer examined */

/* Ensure that the cache has capacity for at least `need` entries */
static void ensure_cache_cap (size_t need)
{
    if (cache_cap < need) {
        size_t newcap = cache_cap ? cache_cap * 2 : 16;
        while (newcap < need) newcap <<= 1;
        cache = mem_resize(seq_t, cache, newcap);
        cache_cap = newcap;
    }
}

/* Append a new value to the cache */
static void cache_append (seq_t val)
{
    ensure_cache_cap(cache_len + 1);
    cache[cache_len++] = val;
}

seq_t a(idx_t n)
{
    /* Lazy initialisation of the cache */
    if (cache == NULL) {
        cache_cap = 16;
        cache = mem_new(seq_t, cache_cap);
        cache_len = 0;
    }

    /* Generate terms until we have at least n of them */
    while (cache_len < n) {
        tested++;

        /* Only test primes; the helper works for any seq_t size */
        if (!is_prime_big((seq_t)tested))
            continue;

        seq_t p = (seq_t)tested;           /* candidate prime            */
        seq_t lim = floor_sqrt(p);         /* largest possible x value   */
        bool good = false;

        /* Search for a representation p = x^2 + y^2 with x,y>0.
           If such x,y exist, check whether q = x^2 + xy + y^2 is prime. */
        for (idx_t ix = 1; ix <= (idx_t)lim; ++ix) {
            seq_t x = (seq_t)ix;
            seq_t x2 = x * x;
            seq_t y2 = p - x2;
            if (y2 == 0) continue;        /* y would be zero */
            seq_t y = floor_sqrt(y2);
            if (y == 0) continue;        /* y would be zero */
            if (y * y != y2) continue;   /* y^2 not exact */

            seq_t q = x2 + x * y + y * y;
            if (is_prime_big(q)) {
                good = true;
                break;
            }
        }

        if (good)
            cache_append(p);
    }

    /* a(1) corresponds to cache[0] */
    return cache[n - 1];
}
