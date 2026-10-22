/*
 * A359551  Dirichlet inverse of A359550, which is multiplicative sequence
 *          with a(p^e) = 1 if e < p, otherwise 0.
 * https://oeis.org/A359551
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  sign, mult
 * Formula:   a(1) = 1; multiplicative. For a prime power p^e, a(p^e) = 1 if e
 *            % p == 0, a(p^e) = -1 if e % p == 1, otherwise a(p^e) = 0
 * Range:     i8 n <= 8952, i16 n <= 8952, i32 n <= 8952, i64 n <= 8952,
 *            i128 n <= 8952, big any n
 * Helpers:   factor_u64
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdint.h>
#include "../utils/factor_u64.h"

seq_t a(idx_t n)
{
    if (n == (idx_t)1)
        return (seq_t)1;

    /* Buffers large enough for the number of distinct prime factors
       of any 64‑bit integer. */
    static uint64_t primes[64];
    static uint64_t exps[64];
    static size_t cnt;

    factor_u64((uint64_t)n, primes, exps, &cnt);

    seq_t result = 1;
    for (size_t i = 0; i < cnt; ++i)
    {
        uint64_t p   = primes[i];
        uint64_t e   = exps[i];
        uint64_t mod = e % p;

        if (mod == 0)
            continue;          /* contribution 1 */
        else if (mod == 1)
            result = -result;   /* multiply by -1 */
        else
            return (seq_t)0;    /* any other case gives 0 */
    }
    return result;
}
