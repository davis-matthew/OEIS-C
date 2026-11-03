/*
 * A386340  Primes without {3, 4} as digits.
 * https://oeis.org/A386340
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base, easy
 * Formula:   not stated
 * Range:     u8 n <= 30, u16 n <= 1740, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Validated: standard, pass, 72 terms through n = 9181 (2026-11-03)
 * Helpers:   is_prime_u64, mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Search: a(n) is the nth prime that contains no digits 3 or 4. */
#include <stdlib.h>
#include <stdbool.h>
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

static bool digits_ok(seq_t x)
{
    while (x > 0)
    {
        unsigned d = (unsigned)(x % 10);
        if (d == 3 || d == 4)
            return false;
        x /= 10;
    }
    return true;
}

seq_t a(idx_t n)
{
    /* Ensure n >= 1. Behaviour for n==0 is undefined as per spec. */
    static seq_t *cache = NULL;     /* cached primes */
    static size_t cache_len = 0;    /* number of cached primes */
    static size_t cache_cap = 0;    /* allocated capacity */
    static seq_t next_candidate = 2;/* next number to test */

    while (cache_len < n)
    {
        if (digits_ok(next_candidate) && is_prime_big(next_candidate))
        {
            if (cache_len == cache_cap)
            {
                size_t new_cap = cache_cap ? cache_cap << 1 : 64;
                seq_t *tmp = mem_resize(seq_t, cache, new_cap);
                if (!tmp)
                    exit(EXIT_FAILURE);
                cache = tmp;
                cache_cap = new_cap;
            }
            cache[cache_len++] = next_candidate;
        }
        ++next_candidate;
    }

    return cache[(size_t)n - 1];
}
