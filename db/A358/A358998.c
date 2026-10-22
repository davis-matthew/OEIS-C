/*
 * A358998  Nonprimes whose sum of factorials of digits is a prime.
 * https://oeis.org/A358998
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  base, easy, nonn
 * Formula:   not stated
 * Range:     u8 n <= 22, u16 n <= 3920, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   is_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A358998: Nonprimes whose sum of factorials of decimal digits is a prime. */

#include <stdbool.h>
#include <stdlib.h>
#include <stdint.h>
#include "../utils/is_prime.h"

static const uint64_t FACTU64[10] = {
    1ULL, 1ULL, 2ULL, 6ULL, 24ULL, 120ULL,
    720ULL, 5040ULL, 40320ULL, 362880ULL
};

/* cache of found terms */
static seq_t *SEQ_CACHE = NULL;
static size_t   CACHE_CAP   = 0;
static size_t   CACHE_SIZE  = 0;      /* number of terms stored */
static idx_t    NEXT_CANDID = 1;      /* next integer to test */

/* sum of digit factorials for an idx_t number (kept in uint64_t). */
static uint64_t digit_factorial_sum_u64(idx_t n)
{
    uint64_t sum = 0;
    if (n == 0)
        return FACTU64[0];
    while (n > 0) {
        unsigned int d = (unsigned int)(n % 10);
        sum += FACTU64[d];
        n /= 10;
    }
    return sum;
}

/* trial division primality test for uint64_t values. */
static bool is_prime_uint64(uint64_t v)
{
    if (v < 2)
        return false;
    if (v % 2 == 0)
        return v == 2;
    for (uint64_t i = 3; i * i <= v; i += 2)
        if (v % i == 0)
            return false;
    return true;
}

seq_t a(idx_t n)
{
    if (n == 0)                 /* undefined for indices < 1 */
        return (seq_t)0;

    while (CACHE_SIZE < (size_t)n) {

        /* Find the next qualifying number. */
        while (true) {
            if (!is_prime(NEXT_CANDID)) {            /* non‑prime */
                uint64_t sum = digit_factorial_sum_u64(NEXT_CANDID);
                if (is_prime_uint64(sum)) {          /* sum is prime */
                    break;                           /* accept */
                }
            }
            NEXT_CANDID++;                          /* try next number */
        }

        /* Ensure capacity for new term. */
        if (CACHE_SIZE == CACHE_CAP) {
            size_t new_cap = (CACHE_CAP == 0 ? 16 : CACHE_CAP * 2);
            seq_t *tmp = (seq_t *)realloc(SEQ_CACHE, new_cap * sizeof(seq_t));
            if (!tmp)
                abort();
            SEQ_CACHE = tmp;
            CACHE_CAP = new_cap;
        }

        SEQ_CACHE[CACHE_SIZE++] = (seq_t)NEXT_CANDID;
        NEXT_CANDID++;                               /* next candidate for future calls */
    }

    return SEQ_CACHE[n - 1];
}
