/*
 * A254766  Fundamental positive solution x = x2(n) of the second class of the
 *          Pell equation x^2 - 2*y^2 = A007522(n), n >=1 (primes congruent to
 *          7 mod 8).
 * https://oeis.org/A254766
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 3*x1 - 4*y1 where (x1,y1) is the minimal positive
 *            solution of the Pell equation x^2 - 2*y^2 = p_n, p_n being the
 *            n-th prime congruent to 7 (mod 8)
 * Range:     u8 n <= 4, u16 n <= 50, u32 n <= 50, u64 n <= 50, u128 n <= 50,
 *            big any n
 * Helpers:   build_primes_upto, floor_sqrt
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Standard headers */
#include <stdint.h>
#include <stdlib.h>

/* Utility to sieve primes up to a given limit */
#include "../utils/build_primes_upto.h"
/* Utility to compute integer square root of a seq_t value */
#include "../utils/floor_sqrt.h"

static uint64_t *primes_all      = NULL;  /* all primes up to current limit */
static size_t     primes_all_cnt = 0;     /* number of primes in primes_all   */
static size_t     primes_limit   = 0;     /* current sieve limit */

/* Primes congruent to 7 (mod 8) */
static uint64_t *primes_mod8      = NULL;
static size_t     primes_mod8_cnt = 0;

/* Expand the sieve until we have at least 'need' primes ≡ 7 (mod 8). */
static void ensure_primes(idx_t need)
{
    while (primes_mod8_cnt < (size_t)need)
    {
        /* Determine a new sieve limit (double the current limit). */
        size_t new_limit = (primes_limit == 0) ? 100 : primes_limit * 2;

        /* Build primes up to the new limit. */
        uint64_t *new_primes = NULL;
        size_t    new_cnt    = 0;
        build_primes_upto(new_limit, &new_primes, &new_cnt);

        /* Release the old list of all primes and replace it. */
        if (primes_all)
            free(primes_all);
        primes_all      = new_primes;
        primes_all_cnt  = new_cnt;
        primes_limit    = new_limit;

        /* Count primes congruent to 7 (mod 8) and rebuild the filter list. */
        size_t count8 = 0;
        for (size_t i = 0; i < primes_all_cnt; ++i)
            if (primes_all[i] % 8 == 7)
                ++count8;

        if (primes_mod8)
            free(primes_mod8);
        primes_mod8 = count8 ? (uint64_t*)malloc(count8 * sizeof(uint64_t)) : NULL;

        size_t idx = 0;
        for (size_t i = 0; i < primes_all_cnt; ++i)
            if (primes_all[i] % 8 == 7)
                primes_mod8[idx++] = primes_all[i];

        primes_mod8_cnt = count8;
    }
}

seq_t a(idx_t n)
{
    /* Make sure the n-th prime congruent to 7 (mod 8) is available. */
    ensure_primes(n);
    uint64_t p = primes_mod8[n - 1];

    /* Search for the minimal positive solution (x1, y1)
       of the Pell equation x^2 - 2*y^2 = p. */
    idx_t  y  = 1;
    seq_t  x1 = 0;
    seq_t  y1 = 0;

    for (;; ++y)
    {
        /* t = p + 2*y^2 */
        seq_t y_sq = (seq_t)2 * (seq_t)y * (seq_t)y;
        seq_t t    = (seq_t)p + y_sq;
        /* Compute integer square root of t. */
        seq_t x = floor_sqrt(t);

        if (x * x == t)
        {
            x1 = x;
            y1 = (seq_t)y;
            break;
        }
    }

    /* Compute the second‑class fundamental solution x2 = 3*x1 - 4*y1. */
    seq_t x2 = (seq_t)3 * x1 - (seq_t)4 * y1;
    return x2;
}
