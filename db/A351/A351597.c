/*
 * A351597  Primes p such that the 6 consecutive primes starting with p are
 *          congruent to 11, 13, 17, 19, 23, 29 (modulo 30) in this order.
 * https://oeis.org/A351597
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 1, u16 n <= 3, u32 n <= 31, u64 n <= 31, u128 n <= 31,
 *            big any n
 * Validated: partial, pass, 31 terms through n = 31 (2026-11-03)
 * Helpers:   is_prime_u128, is_prime_u64, mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  OEIS A351597: Primes p such that the 6 consecutive primes starting with
    p are congruent to 11,13,17,19,23,29 (mod 30) in this order.
    This routine searches for primes one by one, keeping a sliding window
    of the last six primes; whenever the window matches the required residues
    the first prime of that window is cached as the next term.
    The sequence is memoised, so calls with arbitrary n are fast. */

#include <stdbool.h>
#include <stdint.h>
#include <stddef.h>

#include <stdlib.h>
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

/* cache of found terms */
static seq_t *cache = NULL;     /* array of found primes */
static size_t cache_cap = 0;    /* allocated capacity of cache */
static size_t cache_len = 0;    /* number of entries actually stored */

/* sliding window of the last six primes */
static seq_t win[6] = {0};
static size_t win_len = 0;

/* the prime most recently used when searching */
static seq_t last_candidate = 0;

/* returns the first prime strictly greater than start */
static seq_t next_prime(seq_t start)
{
    seq_t n = start + (seq_t)1;
    if (n <= (seq_t)2) return (seq_t)2;
    if ((n & (seq_t)1) == 0) n++;           /* make odd */
    for (;;) {
        if (isPrimeGeneric(n)) return n;
        n += (seq_t)2;                      /* test only odds */
    }
}

/* grow the cache array if needed and append a value */
static void append_cache(seq_t val)
{
    /* ensure capacity for one more element */
    grow_seq(&cache, &cache_cap, cache_len + 1);
    cache[cache_len++] = val;
}

/* required function */
seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0;   /* behaviour below offset is unspecified */

    /* generate terms until we have at least n cached */
    while (cache_len < (size_t)n) {
        /* get the next prime */
        seq_t prime = next_prime(last_candidate);
        last_candidate = prime;

        /* update sliding window */
        if (win_len < 6) {
            win[win_len++] = prime;
        } else {
            /* shift left */
            for (size_t i = 0; i < 5; ++i) win[i] = win[i + 1];
            win[5] = prime;
        }

        /* check pattern when window is full */
        if (win_len == 6) {
            static const seq_t residues[6] = {11, 13, 17, 19, 23, 29};
            bool match = true;
            for (size_t i = 0; i < 6; ++i) {
                if ((win[i] % (seq_t)30) != residues[i]) {
                    match = false;
                    break;
                }
            }
            if (match) {
                /* first prime of the matching block */
                append_cache(win[0]);
            }
        }
    }

    return cache[n - 1];
}
