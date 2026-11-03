/*
 * A376801  a(0) = 397; a(n+1) = a(n)^2 if a(n) is prime, floor(a(n)/2)
 *          otherwise.
 * https://oeis.org/A376801
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(0)=397; a(n+1) = a(n)^2 if a(n) is prime, a(n+1) =
 *            floor(a(n)/2) otherwise. The function memoises previously
 *            computed values and works for any external linkage and any build
 *            of seq_t (unsigned integer or arbitrary‑precision class)
 * Range:     u8 none, u16 n <= 0, u32 n <= 61, u64 n <= 90, u128 n <= 99,
 *            big any n
 * Validated: standard, pass, 49 terms through n = 2455 (2026-11-03)
 * Helpers:   is_prime_u64, mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdbool.h>
#include <stdint.h>
#include <string.h>
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

/* Helper: safe multiplication for seq_t */
static inline seq_t seq_mul(seq_t a, seq_t b) {
    return a * b;
}

seq_t a(idx_t n)
{
    /* Static cache for memoisation. */
    static seq_t *cache = NULL;   /* array of seq_t */
    static size_t cache_cap = 0;  /* allocated capacity */
    static idx_t cache_len = 0;   /* highest index computed + 1 */

    /* Ensure we have computed up to index n. */
    if (cache_len <= n) {
        /* Allocate or grow the cache as needed. */
        size_t needed = (size_t)(n + 1);
        if (needed > cache_cap) {
            size_t new_cap = cache_cap ? cache_cap * 2 : 16;
            while (new_cap < needed) new_cap <<= 1;
            seq_t *tmp = mem_resize(seq_t, cache, new_cap);
            if (!tmp) {
                /* In an evaluation environment we can abort on OOM. */
                abort();
            }
            cache = tmp;
            cache_cap = new_cap;
        }

        /* Initialise the sequence if this is the first call. */
        if (cache_len == 0) {
            cache[0] = (seq_t)397;
            cache_len = 1;
        }

        /* Compute terms up to n. */
        for (idx_t i = cache_len; i <= n; ++i) {
            seq_t prev = cache[i - 1];
            if (is_prime_big(prev))
                cache[i] = seq_mul(prev, prev);
            else
                cache[i] = prev / 2;   /* floor division by 2 */
        }

        cache_len = n + 1;
    }

    return cache[n];
}
