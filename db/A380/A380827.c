/*
 * A380827  Least integer k such that the multiplicative group modulo n is a
 *          subgroup of the symmetric group S_k.
 * https://oeis.org/A380827
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 256, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   factor_u64, pow_uint
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  A380827: least integer k such that the multiplicative group modulo n
    is a subgroup of the symmetric group S_k.
    The value is obtained by factoring n into prime powers.  For an odd
    prime power p^e the contribution is A008475(φ(p^e)) where
    φ(p^e) = (p-1)p^(e-1).  For 2^e the contribution is
        e = 0 or 1 : 0  (the trivial group)
        e = 2     : 2
        e ≥ 3     : 2 + 2^(e-2)
    The overall value is the sum of the contributions of all prime
    powers.  Special cases: a(1)=1, a(2)=1.  The algorithm runs in
    O(log n) time per call. */

#include <stdint.h>
#include "../utils/factor_u64.h"
#include "../utils/pow_uint.h"

static seq_t sum_prime_powers(uint64_t m)
{
    if (m <= 1) return 0;
    uint64_t primes[64];
    uint64_t exps[64];
    size_t cnt = 0;
    factor_u64(m, primes, exps, &cnt);
    seq_t sum = 0;
    for (size_t i = 0; i < cnt; ++i) {
        sum += pow_uint(primes[i], exps[i]); /* q^e */
    }
    return sum;
}

seq_t a(idx_t n)
{
    if (n == 1) return 1;
    if (n == 2) return 1;

    seq_t result = 0;

    uint64_t primes_u64[64];
    uint64_t exps_u64[64];
    size_t cnt = 0;
    factor_u64(n, primes_u64, exps_u64, &cnt);

    for (size_t i = 0; i < cnt; ++i) {
        uint64_t p = primes_u64[i];
        uint64_t e = exps_u64[i];

        if (p == 2) {
            if (e == 1) {
                /* trivial group, no extra degree */
                continue;
            } else if (e == 2) {
                result += 2; /* a(4) */
            } else {
                /* a(2^k) = 2 + 2^(k-2)  for k ≥ 3 */
                result += 2 + pow_uint(2, (idx_t)(e - 2));
            }
            continue;
        }

        /* odd prime power */
        /* compute φ(p^e) = (p-1)*p^(e-1) */
        uint64_t pow_small = 1;
        for (uint64_t j = 1; j < e; ++j) pow_small *= p;
        uint64_t phi = (p - 1) * pow_small;

        seq_t contrib = sum_prime_powers(phi);
        result += contrib;
    }

    return result;
}
