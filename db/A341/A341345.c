/*
 * A341345  a(n) = A048673(n) mod 3.
 * https://oeis.org/A341345
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = A048673(n) mod 3, with A048673(n) = (A003961(n)+1)/2;
 *            computed as 2*(A003961(n) mod 3 + 1) mod 3
 * Range:     u8 n <= 65537, u16 n <= 65537, u32 n <= 65537, u64 n <= 65537,
 *            u128 n <= 65537, big any n
 * Validated: standard, pass, 121 terms through n = 9800 (2026-11-10)
 * Helpers:   factor_u64, next_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stddef.h>
#include <stdbool.h>
#include <stdlib.h>
#include "../utils/factor_u64.h"
#include "../utils/next_prime.h"

seq_t a(idx_t n)
{
    uint64_t primes[64];
    uint64_t exps[64];
    size_t cnt = 0;
    uint64_t r = 1; /* A003961(n) mod 3 */

    if (n > 1)
        factor_u64((uint64_t)n, primes, exps, &cnt);

    for (size_t i = 0; i < cnt; ++i) {
        uint64_t np = next_prime(primes[i]) % 3;
        for (uint64_t k = 0; k < exps[i]; ++k)
            r = (r * np) % 3;
    }
    return (seq_t)((2 * (r + 1)) % 3);
}
