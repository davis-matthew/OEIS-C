/*
 * A372882  a(n) = Sum_{k=1..n} gcd(k^3,n).
 * https://oeis.org/A372882
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, mult
 * Formula:   a(n) = Sum_{k=1..n} gcd(k^3, n)
 * Range:     u8 n <= 26, u16 n <= 863, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   factor_u64
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdint.h>
#include "../utils/factor_u64.h"

seq_t a(idx_t n)
{
    if (n <= 1) return (n == 1) ? (seq_t)1 : (seq_t)0;

    /* Factorize n: primes[i]^exps[i] */
    uint64_t primes[64], exps[64];
    size_t cnt = 0;
    factor_u64(n, primes, exps, &cnt);

    seq_t result = 1;
    for (size_t idx = 0; idx < cnt; ++idx) {
        uint64_t p = primes[idx];
        uint64_t e = exps[idx];
        seq_t p_seq = (seq_t)p;

        /* Pre‑compute powers p^0 … p^e */
        seq_t pow[65];
        pow[0] = 1;
        for (uint64_t i = 1; i <= e; ++i)
            pow[i] = pow[i - 1] * p_seq;

        /* Compute the contribution of p^e */
        seq_t sum = 0;
        for (uint64_t k = 0; k < e; ++k) {
            seq_t diff = pow[e - k] - pow[e - k - 1];
            uint64_t exp = (3 * k <= e) ? (3 * k) : e;
            sum += diff * pow[exp];
        }
        seq_t part = pow[e] + sum;
        result *= part;
    }
    return result;
}
