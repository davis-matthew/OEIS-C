/*
 * A295435  a(n) = (12*n)!*(3*n)!/((6*n)!*(5*n)!*(4*n)!).
 * https://oeis.org/A295435
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = (12*n)!*(3*n)!/((6*n)!*(5*n)!*(4*n)!) computed as the
 *            product of prime powers exponent = ep(12n) - ep(6n) - ep(5n) -
 *            ep(4n) + ep(3n)
 * Range:     u8 n <= 0, u16 n <= 1, u32 n <= 2, u64 n <= 5, u128 n <= 10,
 *            big any n
 * Validated: partial, pass, 12 terms through n = 11 (2026-11-01)
 * Helpers:   build_primes, ipow_idx
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  helpers from the utils library  */
#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>
#include <string.h>
#include <stddef.h>
#include "../utils/build_primes.h"
#include "../utils/ipow_idx.h"

/* Helper: safe multiplication for seq_t */
static inline seq_t seq_mul(seq_t a, seq_t b) {
    return a * b;
}

/* Count exponent of prime p in m! */
static inline idx_t count_factor_in_factorial(idx_t m, idx_t p)
{
    idx_t cnt = 0;
    while (m)
    {
        m /= p;
        cnt += m;
    }
    return cnt;
}

seq_t a(idx_t n)
{
    if (n == 0)
        return (seq_t)1;

    idx_t limit = 12 * n;

    /* get all primes up to limit */
    size_t primes_cnt = 0;
    idx_t *primes = NULL;
    build_primes(limit, &primes, &primes_cnt);

    seq_t result = (seq_t)1;

    for (size_t i = 0; i < primes_cnt; ++i)
    {
        idx_t p = primes[i];

        idx_t ep12 = count_factor_in_factorial(12 * n, p);
        idx_t ep6  = count_factor_in_factorial(6 * n,  p);
        idx_t ep5  = count_factor_in_factorial(5 * n,  p);
        idx_t ep4  = count_factor_in_factorial(4 * n,  p);
        idx_t ep3  = count_factor_in_factorial(3 * n,  p);
        idx_t exp  = ep12 - ep6 - ep5 - ep4 + ep3;

        if (exp)
        {
            seq_t pow_p = ipow_idx((seq_t)p, exp);
            result = seq_mul(result, pow_p);
        }
    }

    free(primes);
    return result;
}
