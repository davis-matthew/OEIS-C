/*
 * A241059  Antipalindromic primes: primes with an even number of digits such
 *          that the digits in the first half of the prime differ from the
 *          corresponding digits of the second half.
 * https://oeis.org/A241059
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 n <= 20, u16 n <= 856, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Validated: standard, pass, 66 terms through n = 9956 (2026-11-03)
 * Helpers:   is_prime_u128, is_prime_u64, mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Antipalindromic primes: a(n) is the n‑th prime that has an even number
   of decimal digits and whose first half of the digits differ from the
   corresponding digits of the second half.  
   The implementation searches for primes progressively and caches the
   resulting antipalindromic primes so that repeated calls are fast. */

#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>
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

/* Returns true iff p is an antipalindromic prime. */
static bool is_antipal_prime(seq_t p)
{
    /* Buffer large enough for most primes that arise in the limits. */
    uint8_t buf[256];
    int d = 0;
    seq_t tmp = p;

    /* Extract decimal digits into buf, least‑significant first. */
    while (tmp > 0) {
        buf[d++] = (uint8_t)(tmp % 10);
        tmp     /= 10;
    }

    /* Primes with odd numbers of digits cannot satisfy the condition. */
    if (d % 2 != 0)
        return false;

    int half = d / 2;
    for (int i = 0; i < half; ++i) {
        if (buf[i] == buf[d - 1 - i])
            return false;         /* matching digits found */
    }
    return true;                  /* all corresponding digits differ */
}

/* Forward declarations */
static bool is_antipal_prime(seq_t p);
static void append_to_cache(seq_t val);

/* Cache of antipalindromic primes. It grows dynamically as needed. */
static seq_t  *cache      = NULL;   /* array of results          */
static size_t  cache_len  = 0;      /* how many have been stored */
static size_t  cache_cap  = 0;      /* current allocated capacity*/
static seq_t   last_checked = 2;    /* maximum value processed   */
static bool    initialized = false;

seq_t a(idx_t n)
{
    /* Initialise on first call. */
    if (!initialized) {
        initialized    = true;
        last_checked   = 2;          /* 2 is the first prime examined */
    }

    /* Expand the cache until we have at least n entries. */
    while (cache_len < n) {
        /* Candidate starts after the previously examined number. */
        seq_t cand = last_checked + 1;

        /* Skip even numbers (except 2 which is already processed). */
        if (cand % 2 == 0)
            cand++;

        /* Find the next antipalindromic prime. */
        for (;;) {
            if (!isPrimeGeneric(cand))
                cand += 2;          /* skip to next odd candidate */
            else if (is_antipal_prime(cand)) {
                append_to_cache(cand);
                last_checked = cand;
                break;              /* found one, continue outer loop */
            } else
                cand += 2;          /* skip to next odd */
        }
    }

    return cache[n - 1];
}

/* Append a value to the cache, enlarging the array if necessary. */
static void append_to_cache(seq_t val)
{
    if (cache_len == cache_cap) {
        size_t new_cap = cache_cap == 0 ? 4 : cache_cap * 2;
        seq_t *tmp = mem_resize(seq_t, cache, new_cap);
        /* In the unlikely event of realloc failure tmp will be NULL.
           For the purposes of this assignment we assume success.   */
        cache   = tmp;
        cache_cap = new_cap;
    }
    cache[cache_len++] = val;
}
