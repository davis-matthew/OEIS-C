/*
 * A377991  Numbers k such that A351568(k) and A351569(k) are not coprime,
 *          where A351568 and A351569 are the sum of divisors of the largest
 *          unitary divisor of n that is a square, and of the largest unitary
 *          divisor of n that is an exponentially odd number, respectively.
 * https://oeis.org/A377991
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 1, u16 n <= 43, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   factor_u64, gcd, pow_uint
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence: A377991 – numbers k such that gcd(A351568(k),A351569(k)) > 1.
   Computed by brute‑force enumeration of integers. */

#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>

#include "../utils/factor_u64.h"
#include "../utils/gcd.h"
#include "../utils/pow_uint.h"

/* Cache for the sequence values */
static seq_t *cache      = NULL;
static size_t cache_len  = 0;
static size_t cache_cap  = 0;

/* Next integer to test (initially 0 so that the first test is 1) */
static uint64_t test_next = 0;

/* Append a new term to the cache, enlarging it if necessary. */
static void append_term(seq_t val)
{
    if (cache_len >= cache_cap) {
        size_t new_cap = cache_cap ? cache_cap * 2 : 16;
        seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        if (!tmp) abort();                     /* out of memory */
        cache = tmp;
        cache_cap = new_cap;
    }
    cache[cache_len++] = val;
}

/* Return the nth term of A377991 (offset 1) */
seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0;                 /* defensive */

    size_t wanted = (size_t)n;
    while (cache_len < wanted) {
        /* Test the next integer */
        ++test_next;

        /* Factor test_next */
        uint64_t pr[64];
        uint64_t ex[64];
        size_t cnt = 0;
        factor_u64(test_next, pr, ex, &cnt);

        /* Compute sigma of the largest unitary divisor that is a square */
        seq_t sigma_square = 1;
        /* Compute sigma of the largest unitary divisor that is exponentially odd */
        seq_t sigma_odd = 1;

        for (size_t i = 0; i < cnt; ++i) {
            uint64_t p   = pr[i];
            uint64_t e   = ex[i];
            /* Σ(p^e) = (p^(e+1) – 1) / (p – 1) */
            seq_t p_pow   = pow_uint(p, (idx_t)(e + 1));
            seq_t numer   = p_pow - (seq_t)1;
            seq_t denom   = (seq_t)p - (seq_t)1;
            seq_t sig_p   = numer / denom;

            if ((e & 1ULL) == 0) {            /* even exponent → square part */
                sigma_square *= sig_p;
            } else {                         /* odd exponent  → odd part */
                sigma_odd *= sig_p;
            }
        }

        /* If the gcd of the two sigma values > 1, include this integer. */
        seq_t g = gcd(sigma_square, sigma_odd);
        if (g > (seq_t)1) {
            append_term((seq_t)test_next);
        }
    }

    return cache[n - 1];
}
