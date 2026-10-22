/*
 * A331970  The sum of the proper bi-unitary divisors of n.
 * https://oeis.org/A331970
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = A188999(n) - n * * Here A188999(n) is the sum of all
 *            bi‑unitary divisors of n. * A bi‑unitary divisor of n is a
 *            divisor d such that for each prime * p dividing n the exponent
 *            of p in d equals 0, e, or e/2 (when e * is even). Equivalently,
 *            d is a bi‑unitary divisor iff for every * prime fac...
 * Range:     u8 n <= 167, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   factor_u64
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/factor_u64.h"

seq_t a(idx_t n) {
    if (n == 0) return 0;   /* harmless defensive guard */

    /* Factorize n */
    uint64_t primes[64];
    uint64_t exps[64];
    size_t cnt = 0;
    factor_u64((uint64_t)n, primes, exps, &cnt);

    /* Compute product of sums of allowed powers for each prime factor */
    seq_t total = 1;
    for (size_t i = 0; i < cnt; ++i) {
        uint64_t p  = primes[i];
        uint64_t e  = exps[i];
        seq_t    base = (seq_t)p;
        seq_t    pow   = 1;   /* p^0 */
        seq_t    sum   = 1;   /* start with exponent 0 */

        /* If e is even, skip the mid exponent e/2 */
        int skip_mid = (e % 2 == 0) ? (int)(e / 2) : -1;

        for (uint64_t f = 1; f <= e; ++f) {
            pow = pow * base;
            if ((int)f != skip_mid) {
                sum = sum + pow;
            }
        }
        total = total * sum;
    }

    /* Convert n to seq_t for subtraction */
    seq_t n_seq = (seq_t)n;
    return total - n_seq;
}
