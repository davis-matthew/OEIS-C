/*
 * A319986  Fully multiplicative with a(p^e) = prime(p mod 6)^e.
 * https://oeis.org/A319986
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, mult
 * Formula:   a(n) = Π_{p^e||n} prime(p mod 6)^e where prime(k) is the k-th
 *            prime (prime(1)=2, prime(2)=3, …)
 * Range:     u8 n <= 39, u16 n <= 1279, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   factor_u64
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stddef.h>
#include "../utils/factor_u64.h"

static const uint64_t pmod_to_primemap[6] = {0, 2, 3, 5, 7, 11};

seq_t a(idx_t n)
{
    if (n <= 1)
        return (seq_t)1;          /* a(1) = 1, a(0) undefined but return 1 */

    uint64_t nn = (uint64_t)n;
    uint64_t primes[64];
    uint64_t exps[64];
    size_t   num = 0;

    factor_u64(nn, primes, exps, &num);

    seq_t result = 1;

    for (size_t i = 0; i < num; ++i) {
        uint64_t p = primes[i];
        uint64_t e = exps[i];
        uint64_t r = p % 6;                /* 1,2,3,5 for primes */
        seq_t mapped = (seq_t)(pmod_to_primemap[r]);

        seq_t pow_val = 1;
        for (uint64_t j = 0; j < e; ++j)
            pow_val = pow_val * mapped;

        result = result * pow_val;
    }

    return result;
}
