/*
 * A327528  Quotient of n over the maximum uniform divisor of n.
 * https://oeis.org/A327528
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = n / (maximum uniform divisor of n)
 * Range:     u8 n <= 2043, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   factor_u64, pow_seq
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include "../utils/factor_u64.h"
#include "../utils/pow_seq.h"

seq_t a(idx_t n) {
    if (n <= 1) return (seq_t)1;
    uint64_t nn = (uint64_t)n;

    /* Factor n into primes and exponents.  The maximum number of distinct primes
       for a 64‑bit integer is strictly less than 64. */
    uint64_t primes[64];
    uint64_t exps[64];
    size_t cnt;
    factor_u64(nn, primes, exps, &cnt);

    seq_t max_d = 1;  /* 1 is always a uniform divisor */

    /* For each distinct exponent value e in the factorization
       (the exponent of the candidate uniform divisor), compute the
       product of all primes with exponent ≥ e, all raised to e. */
    for (size_t ei = 0; ei < cnt; ++ei) {
        int e = (int)exps[ei];
        seq_t cur = 1;
        for (size_t j = 0; j < cnt; ++j) {
            if (exps[j] >= e) {
                cur *= pow_seq((seq_t)primes[j], e);
            }
        }
        if (cur > max_d) {
            max_d = cur;
        }
    }

    /* The desired value is n divided by the maximum uniform divisor. */
    return (seq_t)n / max_d;
}
