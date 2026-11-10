/*
 * A341346  a(n) = A048673(2n-1) mod 3.
 * https://oeis.org/A341346
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = A048673(2n-1) mod 3, where A048673(m) = (A003961(m)+1)/2;
 *            computed via P = A003961(2n-1) mod 3, a(n) = 2*(P+1) mod 3
 * Range:     u8 n <= 65537, u16 n <= 65537, u32 n <= 65537, u64 n <= 65537,
 *            u128 n <= 65537, big any n
 * Validated: standard, pass, 121 terms through n = 9931 (2026-11-10)
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
    uint64_t m = (uint64_t)2 * (uint64_t)n - 1;
    uint64_t primes[64];
    uint64_t exps[64];
    size_t cnt = 0;
    uint64_t P = 1; /* product of shifted primes, mod 3 */

    if (m > 1)
        factor_u64(m, primes, exps, &cnt);

    for (size_t i = 0; i < cnt; ++i) {
        uint64_t np = next_prime(primes[i]) % 3;
        for (uint64_t k = 0; k < exps[i]; ++k)
            P = (P * np) % 3;
    }
    return (seq_t)((2 * (P + 1)) % 3);
}
