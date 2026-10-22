/*
 * A321821  a(n) = Sum_{d|n, d==1 mod 4} d^5 - Sum_{d|n, d==3 mod 4} d^5.
 * https://oeis.org/A321821
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  sign, easy, mult
 * Formula:   not stated
 * Range:     i8 n <= 2, i16 n <= 8, i32 n <= 74, i64 n <= 6208,
 *            i128 n <= 10000, big any n
 * Helpers:   factor_u64
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* OEIS A321821: a(n) = Sum_{d|n, d==1 mod 4} d^5 - Sum_{d|n, d==3 mod 4} d^5 */

#include <stdint.h>
#include <stdbool.h>
#include "../utils/factor_u64.h"

seq_t a(idx_t n)
{
    /* Reduce n to its odd part (A000265(n)) */
    uint64_t m = n;
    while ((m & 1ULL) == 0) {
        m >>= 1;
    }
    if (m == 1) {
        return (seq_t)1;   /* a(2^e) = 1 */
    }

    /* Factor the odd part; primes up to n fit in uint64_t buffer */
    uint64_t primes[64];
    uint64_t exps[64];
    size_t cnt = 0;
    factor_u64(m, primes, exps, &cnt);

    seq_t result = 1;

    for (size_t i = 0; i < cnt; ++i) {
        uint64_t p = primes[i];
        uint64_t e = exps[i];

        /* Compute p^5 (fits in seq_t for all primes that keep a(n) within seq_t) */
        seq_t p5 = (seq_t)p;
        p5 *= (seq_t)p;
        p5 *= (seq_t)p;
        p5 *= (seq_t)p;
        p5 *= (seq_t)p;

        /* Sum of powers with appropriate sign */
        seq_t term = (seq_t)1;
        seq_t power = (seq_t)1;
        for (idx_t k = 1; k <= e; ++k) {
            power *= p5;              /* power = p5^k */

            if ((p & 3ULL) == 1) {    /* p ≡ 1 (mod 4) */
                term += power;
            } else {                  /* p ≡ 3 (mod 4) */
                if (k & 1ULL) {
                    term -= power;
                } else {
                    term += power;
                }
            }
        }

        result *= term;
    }

    return result;
}
