/*
 * A386339  Primes without {2, 9} as digits.
 * https://oeis.org/A386339
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base, easy
 * Formula:   not stated
 * Range:     u8 n <= 29, u16 n <= 2110, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Validated: standard, pass, 72 terms through n = 9973 (2026-11-03)
 * Helpers:   is_prime_u64, mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computes OEIS sequence A386339: primes whose decimal representation
 * contains no digit 2 or 9.  The function iteratively tests odd
 * candidates and keeps a cache of discovered terms, so repeated calls
 * are fast. */
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

/* Check whether a number contains the forbidden digits 2 or 9
 * (works for any seq_t size). */
static bool has_digit_2_or_9(seq_t x)
{
    if (x == 0) return false;
    while (x != 0) {
        uint64_t d = (uint64_t)(x % 10);
        if (d == 2 || d == 9) return true;
        x /= 10;
    }
    return false;
}

seq_t a(idx_t n)
{
    /* Local static structures that cache already found terms. */
    static seq_t *cache = NULL;     /* array of primes found */
    static size_t   cap   = 0;      /* current capacity of cache    */
    static size_t   len   = 0;      /* number of primes stored      */
    static uint64_t cand  = 3;      /* next candidate (64‑bit counter) */

    /* Allocate first cache block if needed. */
    if (cache == NULL) {
        cap = 64;
        cache = mem_new(seq_t, cap);
    }

    /* Generate primes until we have at least n terms cached. */
    while (len < n) {
        /* Skip candidates containing digits 2 or 9. */
        if (has_digit_2_or_9((seq_t)cand)) {
            cand += 2;
            continue;
        }

        /* Test candidate for primality. */
        if (is_prime_big((seq_t)cand)) {
            /* Grow cache if necessary. */
            if (len == cap) {
                cap <<= 1;
                seq_t *tmp = mem_resize(seq_t, cache, cap);
                if (tmp == NULL) {
                    /* Out of memory – abort the program. */
                    exit(EXIT_FAILURE);
                }
                cache = tmp;
            }
            cache[len++] = (seq_t)cand;
        }
        cand += 2;  /* next odd number */
    }

    return cache[n - 1];
}
