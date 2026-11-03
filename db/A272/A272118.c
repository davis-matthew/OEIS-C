/*
 * A272118  Numbers k such that abs(6*k^2 - 342*k + 4903) is prime.
 * https://oeis.org/A272118
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 none, u16 n <= 566, u32 n <= 3874, u64 n <= 3874,
 *            u128 n <= 3874, big any n
 * Validated: standard, pass, 83 terms through n = 3666 (2026-11-03)
 * Helpers:   is_prime_u64, mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A272118: numbers k such that abs(6*k*k - 342*k + 4903) is prime. */
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

seq_t a(idx_t n)
{
    /* Cache of the first n values found so far. */
    static idx_t count = 0;          /* how many have been stored */
    static uint64_t last_k = 0;      /* next k to test */
    static seq_t *cache = NULL;      /* array of found k's */
    static size_t cap   = 0;         /* allocated capacity of cache */

    /* Allocate cache on first call. */
    if (cache == NULL) {
        cap   = 64;
        cache = mem_new(seq_t, cap);
        if (!cache) return 0;        /* Out of memory → return 0. */
    }

    /* Produce more terms until we have at least n. */
    while (count < n) {
        /* Compute f(k) = 6*k^2 - 342*k + 4903 in signed 64‑bit. */
        uint64_t k64 = last_k;
        int64_t val = (int64_t)6 * (int64_t)k64 * (int64_t)k64
                    - (int64_t)342 * (int64_t)k64
                    + (int64_t)4903;

        /* Take absolute value. */
        uint64_t abs_val = (val >= 0) ? (uint64_t)val : (uint64_t)-val;

        /* Test primality.  The utility accepts the same type as seq_t. */
        if (is_prime_big((seq_t)abs_val)) {
            /* Grow cache if needed. */
            if (count >= cap) {
                size_t new_cap = cap * 2 + 64;
                seq_t *tmp = mem_resize(seq_t, cache, new_cap);
                if (!tmp) return 0; /* Out of memory → return 0. */
                cache = tmp;
                cap = new_cap;
            }
            cache[count++] = (seq_t)last_k;
        }
        ++last_k;
    }

    return cache[n - 1];
}
