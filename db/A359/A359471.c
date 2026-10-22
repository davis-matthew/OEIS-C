/*
 * A359471  a(n) = 1 if the product of exponents in the prime factorization of
 *          n is less than 3, otherwise 0.
 * https://oeis.org/A359471
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 1 if the product of exponents in the prime factorization
 *            of n is < 3; otherwise 0
 * Range:     u8 n <= 9560, u16 n <= 9560, u32 n <= 9560, u64 n <= 9560,
 *            u128 n <= 9560, big any n
 * Helpers:   factor_u64
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stddef.h>
#include "../utils/factor_u64.h"

seq_t a(idx_t n)
{
    /* 1 is a trivial case (empty prime factorization → product = 1). */
    if (n <= 1)
        return (seq_t)1;

    uint64_t m = (uint64_t)n;

    /* Temporary buffers – 64 slots are plenty for 64‑bit integers. */
    uint64_t primes[64];
    uint64_t exps[64];
    size_t cnt = 0;

    /* Factorize m.  factor_u64 fills primes[], exps[], and cnt. */
    factor_u64(m, primes, exps, &cnt);

    unsigned two_cnt = 0;       /* number of exponents equal to 2 */

    for (size_t i = 0; i < cnt; ++i) {
        uint64_t e = exps[i];

        if (e >= 3)          /* any exponent ≥3 makes the product ≥3 */
            return (seq_t)0;

        if (e == 2) {
            if (++two_cnt > 1)   /* two or more exponents equal to 2 → product ≥4 */
                return (seq_t)0;
        }
        /* exponents equal to 1 do nothing */
    }

    return (seq_t)1;   /* product is 1 or 2, both < 3 */
}
