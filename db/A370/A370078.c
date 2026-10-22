/*
 * A370078  a(n) = log_2(A370077(n)).
 * https://oeis.org/A370078
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = ∑_{p^e∥n, e a power of 2} log₂(e) (equivalently, a(n) =
 *            sum over prime exponents e of k where e = 2^k, else 0)
 * Range:     u8 n <= 10000, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   factor_u64
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stddef.h>
#include "../utils/factor_u64.h"

seq_t a(idx_t n)
{
    if (n < 1) return (seq_t)0;

    /* factorize n */
    uint64_t nn = (uint64_t)n;
    uint64_t primes[64];
    uint64_t exps[64];
    size_t cnt = 0;
    factor_u64(nn, primes, exps, &cnt);

    seq_t sum = 0;
    for (size_t i = 0; i < cnt; ++i) {
        uint64_t e = exps[i];
        /* e is always >= 1.  Check whether it is a power of 2. */
        if ((e & (e - 1)) == 0) {
            /* e = 2^k  ⇒  log₂(e) = k  */
            seq_t k = 0;
            uint64_t t = e;
            while (t > 1) {
                t >>= 1;
                k++;
            }
            sum += k;
        }
    }
    return sum;
}
