/*
 * A386327  Primes without {0, 9} as digits.
 * https://oeis.org/A386327
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base, easy
 * Formula:   not stated
 * Range:     u8 n <= 35, u16 n <= 2587, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   get_digits, grow_seqt_array
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*
 * Sequence: primes that do not contain the decimal digits 0 or 9.
 * The n‑th term a(n) is the n‑th such prime (a(1)=2).
 *
 * Implementation details: a simple prime generator with trial division
 * and a digit test using the supplied helper get_digits().
 * The function keeps a cache of primes and of those that satisfy
 * the digit restriction, so repeated calls are fast.
 */

#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>
#include "../utils/get_digits.h"
#include "../utils/grow_seqt_array.h"

static uint64_t *prime_list      = NULL;
static size_t    prime_len       = 0;
static size_t    prime_cap       = 0;

static seq_t*    allowed_primes   = NULL; /* primes meeting digit rule */
static size_t   allowed_len       = 0;
static size_t   allowed_cap       = 0;

static uint64_t  last_checked     = 1;  /* last number examined for primality */

/* Grow a uint64_t array */
static void grow_uint64_array(uint64_t **arr, size_t *cap, size_t needed)
{
    if (*cap < needed) {
        size_t new_cap = *cap ? *cap * 2 : 16;
        while (new_cap < needed) new_cap <<= 1;
        *arr = (uint64_t *)realloc(*arr, new_cap * sizeof(uint64_t));
        *cap = new_cap;
    }
}

/* Test whether a number's decimal digits contain 0 or 9. */
static bool digits_ok(seq_t x)
{
    uint8_t buf[32];
    size_t len = 0;
    get_digits(x, buf, &len);
    for (size_t i = 0; i < len; ++i) {
        if (buf[i] == 0 || buf[i] == 9) return false;
    }
    return true;
}

/* Trial division prime test using the primes we have generated. */
static bool is_prime_uint64(uint64_t n)
{
    if (n < 2) return false;
    if (n == 2 || n == 3) return true;
    if (n % 2 == 0) return false;

    for (size_t i = 0; i < prime_len; ++i) {
        uint64_t p = prime_list[i];
        if (p == 2) continue;          /* already handled */
        if (p > n / p) break;           /* p*p > n */
        if (n % p == 0) return false;
    }
    return true;
}

/* Ensure we have at least n primes that satisfy the digit rule. */
static void ensure_n_primes(size_t n)
{
    if (allowed_len >= n) return;

    /* Initialise prime list with first prime if empty. */
    if (prime_len == 0) {
        grow_uint64_array(&prime_list, &prime_cap, 1);
        prime_list[prime_len++] = 2;
        if (digits_ok((seq_t)2)) {
            grow_seqt_array(&allowed_primes, &allowed_cap, 1);
            allowed_primes[allowed_len++] = (seq_t)2;
        }
        last_checked = 2;
    }

    uint64_t candidate = last_checked + 1;
    while (allowed_len < n) {
        if (is_prime_uint64(candidate)) {
            /* store the prime */
            grow_uint64_array(&prime_list, &prime_cap, prime_len + 1);
            prime_list[prime_len++] = candidate;

            /* if it passes the digit test, store it in the allowed list */
            if (digits_ok((seq_t)candidate)) {
                grow_seqt_array(&allowed_primes, &allowed_cap, allowed_len + 1);
                allowed_primes[allowed_len++] = (seq_t)candidate;
            }
        }
        ++candidate;
    }
    last_checked = candidate - 1;
}

/* Public sequence function */
seq_t a(idx_t n)
{
    if (n == 0) return 0;              /* offset is 1 */
    ensure_n_primes((size_t)n);
    return allowed_primes[n - 1];
}
