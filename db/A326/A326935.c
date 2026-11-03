/*
 * A326935  a(1) = 1; a(n) = the largest prime factor of a(n-1) if a(n-1) is a
 *          composite number, otherwise a(n)=a(n-1)+n.
 * https://oeis.org/A326935
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, look
 * Formula:   not stated
 * Range:     u8 n <= 166, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Validated: standard, pass, 86 terms through n = 7344 (2026-11-03)
 * Helpers:   is_prime_u64, mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A326935
   a(1) = 1;
   a(n) = a(n-1) + n                if a(n-1) is 1 or prime,
   a(n) = largest_prime_factor(a(n-1))   otherwise (a(n-1) composite)
   The function memoises results to handle arbitrary call order.
*/

#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>
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

/* Return the largest prime divisor of m (m >= 2). */
static uint64_t largest_prime_divisor(uint64_t m)
{
    uint64_t largest = 0;
    /* factor 2 */
    while ((m & 1ULL) == 0) {
        largest = 2;
        m >>= 1;
    }
    /* odd factors */
    for (uint64_t f = 3; f <= m / f; f += 2) {
        if (m % f == 0) {
            largest = f;
            while (m % f == 0)
                m /= f;
        }
    }
    if (m > 1)            /* remaining m is prime */
        largest = m;
    return largest;
}

/* cache for memoised values */
static seq_t *cache = NULL;       /* array of a(k), 0‑based (a(1) -> cache[0]) */
static size_t cache_len = 0;      /* number of computed terms */
static size_t cache_cap = 0;      /* allocated capacity */

/* ensure the cache can hold at least n terms */
static void ensure_cache_cap(size_t n)
{
    if (cache_cap >= n)
        return;
    /* increase capacity by doubling until adequate */
    size_t new_cap = cache_cap ? cache_cap : 128;
    while (new_cap < n)
        new_cap <<= 1;
    cache = mem_resize(seq_t, cache, new_cap);
    cache_cap = new_cap;
}

/* return a(n) with memoisation */
seq_t a(idx_t n)
{
    if (n == 0) { /* offset starts at 1, but guard for safety */
        return (seq_t)0;
    }

    if (cache == NULL) {
        cache_cap = 128;
        cache = mem_new(seq_t, cache_cap);
        cache_len = 0;
    }

    /* initialise a(1) */
    if (cache_len == 0) {
        cache[0] = (seq_t)1;
        cache_len = 1;
        if (n == 1)
            return cache[0];
    }

    /* compute forward if necessary */
    if (n > cache_len) {
        ensure_cache_cap((size_t)n);
        for (size_t idx = cache_len + 1; idx <= n; ++idx) {
            seq_t prev = cache[idx - 2]; /* a(idx-1) */
            bool composite;

            if (prev == (seq_t)1) {
                composite = false;          /* 1 is not composite */
            } else if (is_prime_big(prev)) {
                composite = false;          /* prime */
            } else {
                composite = true;           /* composite */
            }

            seq_t cur;
            if (composite) {
                /* largest prime factor – safe cast: all terms fit in uint64_t */
                uint64_t prev_u = (uint64_t)prev;
                uint64_t lpf = largest_prime_divisor(prev_u);
                cur = (seq_t)lpf;
            } else {
                cur = prev + (seq_t)idx;     /* index is idx_t (uint64_t) */
            }
            cache[idx - 1] = cur;
        }
        cache_len = n;
    }

    return cache[n - 1];
}
