/*
 * A284374  a(1) = a(2) = 1; a(n) is the largest prime <= (a(n-a(n-1)) +
 *          a(n-a(n-2))) for n > 2.
 * https://oeis.org/A284374
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(1) = a(2) = 1; a(n) = largest prime ≤ (a(n-a(n-1)) +
 *            a(n-a(n-2))) for n > 2
 * Range:     u8 n <= 366, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Validated: standard, pass, 89 terms through n = 9918 (2026-11-03)
 * Helpers:   is_prime_u64, mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>
#include <stdlib.h>
#include "../utils/mem.h"
#include "../utils/is_prime_u64.h"

/* Make sure the cache is large enough */
static void ensure_cache(idx_t n, seq_t **cache_ptr, size_t *cap_ptr) {
    seq_t *cache = *cache_ptr;
    size_t cap = *cap_ptr;
    if (cap <= (size_t)n) {
        size_t new_cap = cap ? (cap << 1) : 16;
        while (new_cap <= (size_t)n)
            new_cap <<= 1;
        seq_t *tmp = mem_resize(seq_t, cache, new_cap);
        if (!tmp && new_cap > 0) {
            /* Allocation failed – abort the program. */
            exit(1);
        }
        if (tmp) {
            /* Initialize newly allocated slots to zero. */
            if (!cache) {
                for (size_t i = 0; i < new_cap; ++i)
                    tmp[i] = 0;
            } else {
                for (size_t i = cap; i < new_cap; ++i)
                    tmp[i] = 0;
            }
            *cache_ptr = tmp;
            *cap_ptr = new_cap;
        }
    }
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

static seq_t *cache = NULL;
static size_t cache_cap = 0;

seq_t a(idx_t n) {
    if (n == 1 || n == 2) return (seq_t)1;

    /* Make sure we have room for indices up to n (including 0 for safety). */
    ensure_cache(n, &cache, &cache_cap);

    /* Initialise base cases if not already done. */
    if (cache[1] == 0) cache[1] = (seq_t)1;
    if (cache[2] == 0) cache[2] = (seq_t)1;

    for (idx_t i = 3; i <= n; ++i) {
        if (cache[i] != 0) continue;             /* already computed */

        /* The indices used by the recurrence.  They are guaranteed to be positive. */
        idx_t idx1 = i - (idx_t)cache[i-1];
        idx_t idx2 = i - (idx_t)cache[i-2];

        seq_t sum = cache[idx1] + cache[idx2];
        seq_t val = sum;

        /* Find largest prime ≤ sum.  The gaps between consecutive primes are
           small compared with sum, so a linear search downward is fast enough. */
        while (!is_prime_big(val)) {
            val--;          /* seq_t supports post‑decrement */
        }

        cache[i] = val;
    }

    return cache[n];
}
