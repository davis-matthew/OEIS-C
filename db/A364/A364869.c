/*
 * A364869  Numbers k such that 6*k+1 is the norm of an Eisenstein prime.
 * https://oeis.org/A364869
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 11, u16 n <= 179, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 7996, big any n
 * Validated: standard, pass, 83 terms through n = 9990 (2026-11-03)
 * Helpers:   ipow_i32, is_prime_u64, mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* A364869: Numbers k such that 6*k+1 is prime or a square of a prime ≡ 5 (mod 6). */

#include <stddef.h>
#include <stdbool.h>
#include <stdlib.h>
#include <string.h>
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

seq_t a(idx_t n)
{
    /* Static cache of already generated terms. */
    static seq_t *cache = NULL;
    static size_t cache_len = 0;      /* number of elements stored */
    static size_t cache_cap = 0;      /* allocated capacity */
    static idx_t next_k = 1;          /* next k to test for inclusion */

    /* Ensure the cache has space for at least one more element. */
    if (cache_len == cache_cap) {
        size_t new_cap = cache_cap ? cache_cap * 2 : 16;
        seq_t *tmp = mem_new(seq_t, new_cap);
        if (!tmp) return 0;          /* allocation failure; unlikely in test */
        if (cache) {
            mem_copy(tmp, cache, cache_len * sizeof(seq_t));
            mem_free(cache);
        }
        cache = tmp;
        cache_cap = new_cap;
    }

    /* Generate terms until we have at least n of them. */
    while (cache_len < n) {
        seq_t p = 6 * (seq_t)next_k + 1;
        bool ok = false;

        if (is_prime_big(p)) {
            ok = true;
        } else {
            seq_t r = floor_sqrt(p);
            if (r * r == p && is_prime_big(r) && (r % 6 == 5)) {
                ok = true;
            }
        }

        if (ok) {
            cache[cache_len++] = (seq_t)next_k;
        }
        ++next_k;
    }

    return cache[n - 1];
}
