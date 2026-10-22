/*
 * A386091  Primes having only {1, 2, 4, 5} as digits.
 * https://oeis.org/A386091
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base, easy
 * Formula:   not stated
 * Range:     u8 n <= 8, u16 n <= 95, u32 n <= 44, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   is_prime_big
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* This function returns the n‑th prime that uses only the decimal
   digits {1,2,4,5}.  It is implemented by generating the numbers
   consisting of those digits in lexicographic order and testing
   them for primality; the result is cached so that calls with
   arbitrary indices are fast. */

#include <stdlib.h>
#include <stdbool.h>
#include "../utils/is_prime_big.h"    /* provides bool is_prime_big(seq_t n) */

static const uint8_t ALLOWED[4] = {1, 2, 4, 5};

/* Convert a base‑4 encoded offset into the decimal number whose
   digits are selected from ALLOWED.  The offset is treated as the
   representation in base 4 of a number with 'len' digits. */
static uint64_t number_from_offset(uint64_t len, uint64_t offset)
{
    uint8_t digits[64];               /* enough for all lengths used */
    uint64_t t = offset;
    for (int i = (int)len - 1; i >= 0; --i) {
        digits[i] = ALLOWED[t % 4];
        t /= 4;
    }

    uint64_t val = 0;
    for (uint64_t i = 0; i < len; ++i)
        val = val * 10 + digits[i];
    return val;
}

/* ------------------------------------------------------------------ */
/*                  Cache and on‑demand generation                    */
/* ------------------------------------------------------------------ */
static seq_t *prime_cache = NULL;     /* array of found primes, in order   */
static size_t prime_cap   = 0;        /* current allocated capacity       */
static size_t prime_len   = 0;        /* number of primes stored          */

static uint64_t current_len = 1;      /* length of the numbers being examined */
static uint64_t current_off = 0;      /* offset within the current length */

/* Ensure that prime_cache contains at least 'need' primes. */
static void ensure_cache(idx_t need)
{
    if (prime_len >= (size_t)need)
        return;

    if (prime_cap < (size_t)need) {
        prime_cap = prime_cap ? prime_cap * 2 : 16;
        if (prime_cap < (size_t)need)
            prime_cap = (size_t)need;
        prime_cache = (seq_t *)realloc(prime_cache,
                                       prime_cap * sizeof(seq_t));
    }

    /* Generate primes until we have at least 'need' of them. */
    for (;;) {
        /* Move to next length when the current range is exhausted. */
        if (current_off >= (uint64_t)1 << (2 * current_len)) { /* 4^len = 2^(2*len) */
            ++current_len;
            current_off = 0;
            continue;
        }

        uint64_t cand = number_from_offset(current_len, current_off);
        ++current_off;

        /* Quick filter: only numbers ending in 1 (except 2 and 5) can be prime. */
        if (cand != 2 && cand != 5 && (cand % 10) != 1)
            continue;

        if (is_prime_big((seq_t)cand)) {
            prime_cache[prime_len++] = (seq_t)cand;
            if (prime_len >= (size_t)need)
                return;
        }
    }
}

/* ------------------------------------------------------------------ */
/*                          Exported function                          */
/* ------------------------------------------------------------------ */
seq_t a(idx_t n)
{
    if (n == 0)   /* behaviour for n < 1 is undefined; return 0 */
        return (seq_t)0;

    ensure_cache(n);
    return prime_cache[(size_t)n - 1];
}
