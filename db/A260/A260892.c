/*
 * A260892  Primes having only {1, 7, 8} as digits.
 * https://oeis.org/A260892
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy, base
 * Formula:   not stated
 * Range:     u8 n <= 5, u16 n <= 38, u32 n <= 4209, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Validated: standard, pass, 59 terms through n = 8436 (2026-11-03)
 * Helpers:   is_prime_u128, is_prime_u64, mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computes the n‑th prime whose decimal digits are only 1, 7, 8
   (OEIS sequence A260892).  The function builds a cache of primes
   found with those digits and grows it lazily until it has at least
   n entries. */
#include <stddef.h>
#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>
#include <limits.h>
#include "../utils/is_prime_u128.h"
#include "../utils/is_prime_u64.h"
#include "../utils/mem.h"

/* -----------------  Generic primality test ------------------------ */
static bool isPrimeGeneric(seq_t n)
{
    /* all seq_t are unsigned; use the smallest type that fits */
    if ((seq_t)(ULLONG_MAX) >= n) {
        return is_prime_u64((uint64_t)n);
    }
#if defined(__SIZEOF_INT128__)
    return is_prime_u128((__uint128_t)n);
#else
    /* fallback: if seq_t > 64‑bit but no 128‑bit support,
       order the test as simple trial division up to sqrt */
    seq_t limit = 0;
    for (seq_t i = 2; i*i <= n; ++i) {
        if (n % i == 0) return false;
    }
    return true;
#endif
}

/* Append a value to the cached array, expanding it if necessary.  `p` is a
   pointer to the array pointer. */
static void append_candidate(seq_t **p, size_t *len, size_t *cap, seq_t val)
{
    if (*len == *cap) {
        size_t newcap = (*cap == 0) ? 16 : (*cap << 1);
        seq_t *tmp = mem_resize(seq_t, *p, newcap);
        if (!tmp) {
            exit(EXIT_FAILURE);
        }
        *p = tmp;
        *cap = newcap;
    }
    (*p)[*len] = val;
    (*len)++;
}

static const uint8_t DIGITS[3] = {1, 7, 8};
static const uint8_t LAST_D[2] = {1, 7};

static seq_t *prime_cache = NULL;
static size_t prime_cache_len = 0;
static size_t prime_cache_cap = 0;
static size_t last_gen_len = 0;

static void rec_generate(seq_t cur, size_t rem)
{
    if (rem == 0) {
        if (isPrimeGeneric(cur))
            append_candidate(&prime_cache, &prime_cache_len, &prime_cache_cap, cur);
        return;
    }
    const uint8_t *opts = (rem == 1) ? LAST_D : DIGITS;
    size_t nopt = (rem == 1) ? 2 : 3;
    for (size_t i = 0; i < nopt; ++i) {
        /* Prevent overflow for native unsigned types */
        if (sizeof(seq_t) <= 8) {
            seq_t digit = (seq_t)opts[i];
            seq_t maxv = ~((seq_t)0);
            if (cur > (maxv - digit) / 10) continue;
        }
        rec_generate(cur * 10 + opts[i], rem - 1);
    }
}

seq_t a(idx_t n)
{
    if (n == 0) return 0; /* undefined for offset 0 */

    if (!prime_cache) {
        prime_cache_cap = 64;
        prime_cache = mem_new(seq_t, prime_cache_cap);
        prime_cache_len = 0;
    }

    while (prime_cache_len < (size_t)n) {
        size_t len = last_gen_len + 1;
        rec_generate(0, len);
        last_gen_len = len;
    }

    return prime_cache[(size_t)n - 1];
}
