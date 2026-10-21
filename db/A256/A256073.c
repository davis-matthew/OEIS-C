/*
 * A256073  Numbers n representable as x*y + x + y, where x >= y > 1, such
 *          that all x's and y's in all representation(s) of n are primes.
 * https://oeis.org/A256073
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 6, u16 n <= 6, u32 n <= 6, u64 n <= 6, u128 n <= 6,
 *            big any n
 * Helpers:   is_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  A256073 – Numbers n representable as x*y+x+y with prime x≥y>1
 *  in all such representations.  Implementation enumerates
 *  candidates m=n+1 and keeps those whose every divisor d>2
 *  (except 1,2,m and m/2 when m even) satisfies that d‑1 is prime.
 *  The first six values are 8,11,15,17,23,31.  The routine caches
 *  terms so that any call order is supported. */

#include <stdint.h>
#include <stdbool.h>
#include <stdlib.h>
#include "../utils/is_prime.h"

typedef uint64_t idx_t;   /* defined by the build */

static seq_t *TERM = NULL;       /* cached terms                 */
static size_t TERM_CAP = 0;      /* allocated capacity           */
static size_t TERM_CNT = 0;      /* number of stored terms       */
static idx_t NEXT_M = 3;         /* next candidate for n+1       */
static bool INIT_DONE = false;   /* one‑time initialization flag */

/* Ensure the term array has at least 'need' slots.  Uses realloc. */
static void ensure_term_cap(size_t need)
{
    if (TERM_CAP < need) {
        size_t new_cap = need * 2 + 10;
        seq_t *tmp = (seq_t *)realloc(TERM, new_cap * sizeof(seq_t));
        if (!tmp) abort();
        TERM = tmp;
        TERM_CAP = new_cap;
    }
}

/* Check whether m=n+1 qualifies.  All divisors >2 (except m/2)
 * must have their minus‑one value prime.  Reject prime m and 2×prime. */
static bool valid_m(idx_t m)
{
    /* Exclude primes and twice a prime */
    if (is_prime(m)) return false;
    if ((m & 1) == 0 && is_prime(m >> 1)) return false;

    bool has_valid = false;
    for (idx_t i = 3; i <= m / i; ++i) {
        if (m % i == 0) {
            idx_t d1 = i;
            idx_t d2 = m / i;

            if (d1 > 2 && d1 < m && d1 != m >> 1) {
                has_valid = true;
                if (!is_prime(d1 - 1)) return false;
            }
            if (d2 > 2 && d2 < m && d2 != m >> 1) {
                has_valid = true;
                if (!is_prime(d2 - 1)) return false;
            }
        }
    }
    return has_valid;
}

/* Main entry point for the sequence */
seq_t a(idx_t n)
{
    if (!INIT_DONE) {
        TERM = NULL;
        TERM_CAP = 0;
        TERM_CNT = 0;
        NEXT_M = 3;
        INIT_DONE = true;
    }

    while (TERM_CNT < n) {
        if (valid_m(NEXT_M)) {
            ensure_term_cap(TERM_CNT + 1);
            TERM[TERM_CNT] = (seq_t)(NEXT_M - 1);
            ++TERM_CNT;
        }
        ++NEXT_M;
    }
    return TERM[n - 1];
}
