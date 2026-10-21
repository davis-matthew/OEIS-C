/*
 * A250206  Least base b > 1 such that b^A000010(n) = 1 (mod n^2).
 * https://oeis.org/A250206
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = least base b > 1 such that b^φ(n) ≡ 1 (mod n²)
 * Range:     u8 n <= 15, u16 n <= 330, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   factor_u64, mod_pow
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/factor_u64.h"
#include "../utils/mod_pow.h"

static inline uint64_t calc_phi_uint64(uint64_t n)
/* Compute φ(n) for n ≥ 1.  The result fits in 64 bits for the
   n-values that will be queried (n ≤ 10 000).  */
{
    uint64_t res = n;
    uint64_t primes[64];
    uint64_t exps[64];
    size_t cnt = 0;
    factor_u64(n, primes, exps, &cnt);
    for (size_t i = 0; i < cnt; ++i) {
        res = res / primes[i] * (primes[i] - 1);
    }
    return res;
}

seq_t a(idx_t n)
{
    if (n == 1u) {
        return (seq_t)2;          /* φ(1) = 1, so answer is 2 */
    }

    uint64_t phi = calc_phi_uint64((uint64_t)n);
    seq_t mod = (seq_t)n * (seq_t)n;

    for (seq_t b = 2; ; ++b) {
        if (mod_pow(b, phi, mod) == (seq_t)1) {
            return b;
        }
    }
}
