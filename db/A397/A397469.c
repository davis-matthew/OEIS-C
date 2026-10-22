/*
 * A397469  a(n) = prime(floor(n*r)), where r = (3 + sqrt(3))/2.
 * https://oeis.org/A397469
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = prime( floor( n*(3+sqrt(3))/2 ) )
 * Range:     u8 n <= 4, u16 n <= 54, u32 n <= 54, u64 n <= 54, u128 n <= 54,
 *            big any n
 * Helpers:   _prime_limit_for_idx, build_primes_upto, floor_sqrt
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stddef.h>
#include <stdlib.h>
#include <stdbool.h>

#include "../utils/floor_sqrt.h"
#include "../utils/_prime_limit_for_idx.h"
#include "../utils/build_primes_upto.h"

/* Compute floor(n*r) where r = (3+sqrt(3))/2 using integer arithmetic.
   The steps are:  floor((3n + floor(n*sqrt(3)))/2) */
static inline seq_t floor_nr(idx_t n)
{
    seq_t n_sq   = (seq_t)n * (seq_t)n;          /* n² */
    seq_t t      = (seq_t)3 * n_sq;              /* 3·n² */
    seq_t s      = floor_sqrt(t);                /* floor(n·sqrt(3)) */
    seq_t combined = (seq_t)3 * (seq_t)n + s;    /* 3·n + floor(n·sqrt(3)) */
    return combined / (seq_t)2;                 /* floor( (3n + s)/2 ) */
}

seq_t a(idx_t n)
{
    if (n == 0)               /* offset rule: n≥1 is expected */
        return 0;

    /* k = floor(n*r) */
    seq_t k     = floor_nr(n);
    idx_t k_idx = (idx_t)k;   /* safe cast: k < 2^64 for all n we handle */

    /* Cached list of primes, persistent across calls. */
    static uint64_t *primes    = NULL;
    static size_t    primes_cnt = 0;

    /* If we don't have enough primes yet, extend the table. */
    if (k_idx > primes_cnt) {
        idx_t limit = _prime_limit_for_idx(k_idx);
        if (primes)
            free(primes);                      /* free old table */
        build_primes_upto(limit, &primes, &primes_cnt);
    }

    /* primes[] is 1‑based in the generated table. */
    return (seq_t)(primes[(size_t)k_idx - 1]);
}
