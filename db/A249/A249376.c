/*
 * A249376  Prime numbers Q such that the concatenation Q,3,Q is prime.
 * https://oeis.org/A249376
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   primes Q such that the concatenation Q,3,Q is prime
 * Range:     u8 none, u16 n <= 8, u32 n <= 728, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Validated: standard, pass, 62 terms through n = 9696 (2026-11-03)
 * Helpers:   is_prime_u128, is_prime_u64, mem, num_digits0
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* The implementation keeps a static cache of found terms and
   uses generic primality testing.  It scans odd numbers, checks
   primality with isPrimeGeneric, builds the concatenated number
   using pow10 and digit_count, tests that for primality, and
   stores each valid Q.  The cache is grown lazily with grow_seq.  */

#include <stddef.h>      /* NULL, size_t */
#include <stdbool.h>     /* bool */

#include <stdint.h>
#include <math.h>
#include <stdlib.h>
#include <limits.h>
#include "../utils/is_prime_u128.h"
#include "../utils/is_prime_u64.h"
#include "../utils/num_digits0.h"
#include "../utils/mem.h"

/* --- dynamic array helpers ------------------------------------------- */
static void grow_seq(seq_t **arr, size_t *cap, size_t need)
{
    if (*cap < need) {
        size_t new_cap = (*cap == 0) ? 64 : (*cap << 1);
        while (new_cap < need) new_cap <<= 1;
        seq_t *tmp = mem_resize(seq_t, *arr, (new_cap * sizeof(**arr)) / sizeof(seq_t));
        if (!tmp) exit(EXIT_FAILURE);
        *arr = tmp;
        *cap = new_cap;
    }
}

/* Compute 10^e for a non‑negative integer exponent e */
static seq_t pow10(idx_t e) {
    seq_t res = 1;
    while (e) {
        res *= 10;
        e--;
    }
    return res;
}

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

seq_t a(idx_t n)
{
    /* Static state */
    static seq_t *cache = NULL;
    static size_t cache_len = 0;
    static size_t cache_cap = 0;
    static uint64_t candidate = 3;  /* start from the first odd prime */

    /* Grow the cache until we have the nth term */
    while (cache_len < (size_t)n) {
        /* Find the next prime candidate */
        while (!isPrimeGeneric((seq_t)candidate))
            candidate += 2;

        /* candidate is now prime Q */
        uint64_t q = candidate;
        seq_t q_seq = (seq_t)q;

        /* Number of decimal digits of Q */
        uint64_t d = num_digits0(q);

        /* 10^d and 10^(d+1) */
        seq_t pow_d  = pow10(d);
        seq_t pow_d1 = pow10(d + 1);

        /* Concatenate Q,3,Q:  Q*10^(d+1) + 3*10^d + Q  */
        seq_t concat = q_seq * pow_d1 + 3 * pow_d + q_seq;

        /* If the concatenated number is prime, store Q */
        if (isPrimeGeneric(concat)) {
            grow_seq(&cache, &cache_cap, cache_len + 1);
            cache[cache_len++] = q_seq;
        }

        /* Advance to the next odd number */
        candidate += 2;
    }

    return cache[(size_t)n - 1];
}
