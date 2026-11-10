/*
 * A378521  Möbius transform of A048673.
 * https://oeis.org/A378521
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(1) = 1; for n > 1, a(n) = phi(A003961(n))/2, where A003961
 *            maps each prime p to the next prime
 * Range:     u8 n <= 95, u16 n <= 3071, u32 n <= 20000, u64 n <= 20000,
 *            u128 n <= 20000, big any n
 * Validated: standard, pass, 97 terms through n = 9929 (2026-11-10)
 * Helpers:   factor_u64, next_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stddef.h>
#include <stdbool.h>
#include "../utils/factor_u64.h"
#include "../utils/next_prime.h"

seq_t a(idx_t n)
{
    if (n <= (idx_t)1) return (seq_t)1;

    uint64_t primes[64];
    uint64_t exps[64];
    size_t cnt = 0;
    factor_u64((uint64_t)n, primes, exps, &cnt);

    seq_t result = (seq_t)1;
    bool halved = false;
    for (size_t i = 0; i < cnt; ++i) {
        uint64_t np = next_prime(primes[i]);   /* odd prime >= 3 */
        uint64_t e = exps[i];
        uint64_t f = np - 1;                   /* even */
        if (!halved) {
            f /= 2;
            halved = true;
        }
        result = result * (seq_t)f;
        for (uint64_t k = 1; k < e; ++k)
            result = result * (seq_t)np;
    }
    return result;
}
