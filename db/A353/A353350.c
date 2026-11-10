/*
 * A353350  a(n) = 1 if A048675(n) is a multiple of 3, otherwise 0.
 * https://oeis.org/A353350
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 1 if A048675(n) == 0 (mod 3), else 0; A048675(n) =
 *            Sum_{p^e || n} e*2^(PrimePi(p)-1), reduced mod 3
 * Range:     u8 n <= 65537, u16 n <= 65537, u32 n <= 65537, u64 n <= 65537,
 *            u128 n <= 65537, big any n
 * Validated: standard, pass, 136 terms through n = 9762 (2026-11-10)
 * Helpers:   factor_u64, prime_pi_u64
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>
#include <stdlib.h>
#include "../utils/factor_u64.h"
#include "../utils/prime_pi_u64.h"

seq_t a(idx_t n)
{
    uint64_t primes[64];
    uint64_t exps[64];
    size_t cnt = 0;
    uint64_t sum = 0;
    size_t i;

    if (n <= 1) return (seq_t)1;
    factor_u64((uint64_t)n, primes, exps, &cnt);
    for (i = 0; i < cnt; ++i) {
        uint64_t pi = prime_pi_u64(primes[i]);
        uint64_t w = ((pi - 1) & 1) ? 2 : 1; /* 2^(pi-1) mod 3 */
        sum += (exps[i] % 3) * w;
    }
    return (seq_t)((sum % 3) == 0);
}
