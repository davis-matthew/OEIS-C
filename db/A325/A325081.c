/*
 * A325081  Prime numbers congruent to 4, 9, 14, 34 or 49 modulo 55
 *          representable by x^2 + x*y + 14*y^2.
 * https://oeis.org/A325081
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 none, u16 n <= 31, u32 n <= 45, u64 n <= 45, u128 n <= 45,
 *            big any n
 * Validated: partial, pass, 45 terms through n = 45 (2026-11-03)
 * Helpers:   ipow_i32, is_prime_u64, mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computes sequence A325081:
   Prime numbers congruent to 4, 9, 14, 34 or 49 modulo 55 that are
   representable by the quadratic form x^2 + x*y + 14*y^2.
   The function returns the n‑th term (offset 1) by iterating over
   integers, testing primality with is_prime_big, and checking the
   representability via the discriminant test:
     Δ = 4*p – 55*y^2
   Δ must be a perfect square and Δ – y must be even.  Results are cached
   so that random access to n works efficiently. */

#include <stdlib.h>
#include <stdbool.h>
#include <stdint.h>

/* Utility functions provided by the build environment */
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

/* Cache of found terms */
static seq_t *cache      = NULL;
static size_t cache_cap  = 0;
static idx_t  cache_len  = 0;   /* number of terms stored in the cache */
static seq_t  last_checked = 1; /* last integer examined */

/* Check whether a prime p can be expressed as x^2 + x*y + 14*y^2. */
static bool representable(seq_t p)
{
    seq_t fourp = p * 4;

    for (seq_t y = 0; ; ++y) {
        /* y^2 */
        seq_t ysq = y * y;

        seq_t sub = 55 * ysq;
        if (sub > fourp) break;          /* 4p – 55*y^2 < 0 */

        seq_t tval = fourp - sub;        /* Δ = 4p – 55*y^2 */
        seq_t t = floor_sqrt(tval);      /* floor sqrt of Δ */

        if (t * t != tval) continue;     /* Δ is not a perfect square */

        /* (Δ – y) must be even for integer x to exist */
        if (((t - y) & 1) == 0) return true;
    }

    return false;
}

seq_t a(idx_t n)
{
    if (n == 0) return 0;   /* the sequence is 1‑based; index 0 unused */

    /* Initialise cache if needed */
    if (cache_cap == 0) {
        cache_cap = 16;
        cache = mem_new(seq_t, cache_cap);
    }

    /* Generate terms until we have at least n of them */
    while (cache_len < n) {
        ++last_checked;   /* next integer to examine */

        /* Skip numbers that cannot be congruent to any of the allowed residues */
        int r = (int)(last_checked % 55);
        if (r != 4 && r != 9 && r != 14 && r != 34 && r != 49)
            continue;

        /* Must be prime */
        if (!is_prime_big(last_checked))
            continue;

        /* Must be representable by the given quadratic form */
        if (representable(last_checked)) {
            /* Expand cache if necessary */
            if (cache_len == cache_cap) {
                cache_cap *= 2;
                cache = mem_resize(seq_t, cache, cache_cap);
            }
            cache[cache_len++] = last_checked;
        }
    }

    /* Return the requested term */
    return cache[n - 1];
}
