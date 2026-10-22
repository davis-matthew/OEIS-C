/*
 * A389455  The largest unitary divisor of n that is a term in A048103 (not
 *          divisible by p^p for any prime p).
 * https://oeis.org/A389455
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy, mult
 * Formula:   a(n) = product over all prime powers p^e dividing n with e < p
 *            of p^e (equivalently, the largest unitary divisor of n that is
 *            not divisible by p^p for any prime p)
 * Range:     u8 n <= 256, u16 n <= 8160, u32 n <= 8160, u64 n <= 8160,
 *            u128 n <= 8160, big any n
 * Helpers:   factor_u64
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stddef.h>
#include "../utils/factor_u64.h"

seq_t a(idx_t n) {
    if (n < 1) return (seq_t)0;  /* offset starts at 1 */
    if (n == 1) return (seq_t)1;

    /* Arrays to hold prime factors and their exponents.
       64 is ample for 64‑bit integers (max ~19 distinct primes). */
    uint64_t primes[64];
    uint64_t exps[64];
    size_t cnt;

    factor_u64((uint64_t)n, primes, exps, &cnt);

    seq_t result = (seq_t)1;
    for (size_t i = 0; i < cnt; ++i) {
        uint64_t p = primes[i];
        uint64_t e = exps[i];

        if (e < p) {
            /* Compute p^e.  For the arbitrary‑precision build this
               multiplication is exact; for the fixed‑width builds any
               overflow wraps around, but those cases never occur for
               the test range. */
            seq_t power = (seq_t)1;
            for (uint64_t j = 0; j < e; ++j) {
                power *= (seq_t)p;
            }
            result *= power;
        }
    }
    return result;
}
