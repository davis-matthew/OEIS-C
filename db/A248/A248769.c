/*
 * A248769  Greatest 5th-power-free divisor of n!.
 * https://oeis.org/A248769
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = ∏p≤n p^( (∑⌊n/p^k⌋) mod 5 )
 * Range:     u8 n <= 5, u16 n <= 9, u32 n <= 18, u64 n <= 37, u128 n <= 60,
 *            big any n
 * Helpers:   build_primes_upto, pow_uint, seq_mul
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Greatest 5th‑power‑free divisor of n! */

#include <stddef.h>   /* size_t */
#include <stdint.h>   /* uint64_t */
#include <stdlib.h>   /* abort, free, realloc */

#include "../utils/build_primes_upto.h"
#include "../utils/pow_uint.h"
#include "../utils/seq_mul.h"

/* --- static data shared between calls ----------------------------------- */
static uint64_t *primes   = NULL;     /* array of primes up to primes_limit */
static size_t   primes_cnt = 0;       /* number of primes in the array   */
static idx_t    primes_limit = 0;     /* maximal value for which primes are cached */

/* --- helper ------------------------------------------------------------- */
static void ensure_primes_upto(idx_t n)
{
    if (n <= primes_limit)
        return;

    /* free previous buffer if it exists */
    if (primes)
        free(primes);

    /* build a new prime list up to n (inclusive) */
    build_primes_upto(n, &primes, &primes_cnt);

    primes_limit = n;
}

/* ------------------------------------------------------------------------
 * Public function required by the exercise
 */
seq_t a(idx_t n)
{
    if (n < 1)            /* guard: OEIS offset starts at 1 */
        return (seq_t)1;

    /* make sure we have all primes ≤ n */
    ensure_primes_upto(n);

    seq_t result = (seq_t)1;

    for (size_t i = 0; i < primes_cnt && primes[i] <= n; ++i) {
        idx_t p = primes[i];
        idx_t m = n;
        uint64_t exp = 0;

        /* Legendre's formula for exponent of prime p in n! */
        while (m) {
            m /= p;
            exp += m;
        }

        uint64_t e = exp % 5;          /* keep only remainder < 5 */
        if (e) {
            seq_t pp = pow_uint((uint64_t)p, e);   /* p^e as seq_t */
            result = seq_mul(result, pp);          /* multiply into product */
        }
    }

    return result;
}
