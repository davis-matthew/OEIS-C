/*
 * A351773  a(n) = Sum_{d|n} Sum_{p|n, p prime} p^d.
 * https://oeis.org/A351773
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Sum_{d|n} Sum_{p|n, p prime} p^d
 * Range:     u8 n <= 4, u16 n <= 6, u32 n <= 10, u64 n <= 16, u128 n <= 23,
 *            big any n
 * Helpers:   big_pow, factor_u64
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/big_pow.h"
#include "../utils/factor_u64.h"

seq_t a(idx_t n) {
    if (n <= 1) return 0;                       /* a(1) = 0 */
    uint64_t n64 = n;

    uint64_t primes[32], exps[32];
    size_t pcnt = 0;                              /* number of prime factors */
    factor_u64(n64, primes, exps, &pcnt);        /* factor n */

    seq_t res = 0;
    for (idx_t d = 1; d <= n; ++d) {
        if (n % d == 0) {
            for (size_t i = 0; i < pcnt; ++i) {
                res += big_pow((seq_t)primes[i], (seq_t)d);
            }
        }
    }
    return res;
}
