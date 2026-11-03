/*
 * A257772  Numbers n>=0 such that (n+1)^3 - n^3 = 3*n^2+3*n+1 is not prime.
 * https://oeis.org/A257772
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 4, u16 n <= 2867, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Validated: standard, pass, 80 terms through n = 9662 (2026-11-03)
 * Helpers:   is_prime_u64, mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* A257772: numbers n≥0 such that 3*n^2 + 3*n + 1 is not prime */

#include <stdbool.h>
#include <stdlib.h>
#include <stdint.h>
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

seq_t a(idx_t n) {
    /* static cache of values */
    static seq_t *cache = NULL;
    static size_t cache_capacity = 0;
    static size_t cache_count   = 0;
    static idx_t next_candidate = 0;  /* next n to test */

    /* grow the cache as needed */
    if (cache == NULL) {
        cache_capacity = 64;
        cache = mem_new(seq_t, cache_capacity);
    }

    /* generate terms until we have at least n terms */
    while (cache_count < (size_t)n) {
        seq_t cand = (seq_t)next_candidate;
        seq_t val = 3 * cand * cand + 3 * cand + 1;
        if (!is_prime_big(val)) {           /* composite or 1 */
            if (cache_count == cache_capacity) {   /* expand */
                cache_capacity <<= 1;
                cache = mem_resize(seq_t, cache, cache_capacity);
            }
            cache[cache_count++] = cand;
        }
        ++next_candidate;
    }

    return cache[(size_t)(n - 1)];
}
