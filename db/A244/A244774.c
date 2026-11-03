/*
 * A244774  Prime numbers ending in the prime number 73.
 * https://oeis.org/A244774
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) is the n-th prime in the arithmetic progression 100*k+73
 * Range:     u8 n <= 2, u16 n <= 164, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Validated: standard, pass, 57 terms through n = 995 (2026-11-03)
 * Helpers:   is_prime_u64, mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

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

/* Cached primes of the form 100*k+73. */
static seq_t *cache = NULL;   /* array of found primes */
static size_t cache_len = 0;  /* number of stored primes */
static size_t cache_cap = 0;  /* current capacity */

/* Append a prime to the cache, expanding it if necessary. */
static void append_prime(seq_t val) {
    if (cache_len == cache_cap) {
        size_t new_cap = (cache_cap == 0) ? 16 : cache_cap * 2;
        seq_t *new_ptr = mem_resize(seq_t, cache, new_cap);
        if (!new_ptr) exit(EXIT_FAILURE); /* out of memory */
        cache = new_ptr;
        cache_cap = new_cap;
    }
    cache[cache_len++] = val;
}

/* Return the n‑th prime ≡ 73 (mod 100) for n ≥ 1. */
seq_t a(idx_t n) {
    if (n == 0) return (seq_t)0;       /* offset 0 is undefined, return 0 */

    /* Initialise cache with the first prime in the progression. */
    if (cache_len == 0) {
        append_prime((seq_t)73);
    }

    /* Extend the cache until it contains at least n primes. */
    while ((idx_t)cache_len < n) {
        seq_t last = cache[cache_len - 1];
        seq_t cand = last + (seq_t)100;   /* next candidate in the progression */

        /* Search forward until a prime is found. */
        while (!is_prime_big(cand)) {
            cand += (seq_t)100;
        }

        append_prime(cand);
    }

    return cache[n - 1];
}
