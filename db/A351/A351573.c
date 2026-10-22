/*
 * A351573  Arithmetic derivative of the largest unitary divisor of n that is
 *          an exponentially odd number.
 * https://oeis.org/A351573
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = A003415(A350389(n))
 * Range:     u8 n <= 87, u16 n <= 8577, u32 n <= 8577, u64 n <= 8577,
 *            u128 n <= 8577, big any n
 * Helpers:   factor_u64, seq_pow
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Implements the arithmetic derivative of the largest unitary divisor of n
   that is an exponentially odd number (i.e., all exponents odd). */

#include <stddef.h>
#include <stdint.h>
#include <stdbool.h>
#include "../utils/factor_u64.h"
#include "../utils/seq_pow.h"

seq_t a(idx_t n)
{
    if (n <= 1)
        return 0;          /* a(1) = 0 */

    /* Factor n.  Since n <= 10 000 for required performance, a small
       static buffer is sufficient for all possible factors. */
    uint64_t primes[32];
    uint64_t exps[32];
    size_t cnt = 0;
    factor_u64((uint64_t)n, primes, exps, &cnt);

    /* d = product of p^e for all prime powers with e odd. */
    seq_t d = 1;
    for (size_t i = 0; i < cnt; ++i) {
        if ((exps[i] & 1) == 1)            /* exponent is odd */
            d *= seq_pow((seq_t)primes[i], (idx_t)exps[i]);
    }

    if (d == 1)   /* the derivative of 1 is 0 */
        return 0;

    /* derivative of d: d * sum(e / p) over odd exponents. */
    seq_t result = 0;
    for (size_t i = 0; i < cnt; ++i) {
        if ((exps[i] & 1) == 1) {
            seq_t term = d * (seq_t)exps[i];
            term /= (seq_t)primes[i];
            result += term;
        }
    }
    return result;
}
