/*
 * A286584  a(n) = A048673(n) mod 4.
 * https://oeis.org/A286584
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = A048673(n) mod 4, where A048673(n) = (A003961(n)+1)/2.
 *            A003961(n) is odd, so a(n) = ((A003961(n) mod 8) + 1) / 2
 * Range:     u8 n <= 10000, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Validated: standard, pass, 129 terms through n = 9851 (2026-11-08)
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
    if (n <= 1)
        return (seq_t)1;

    uint64_t primes[64];
    uint64_t exps[64];
    size_t cnt = 0;

    factor_u64((uint64_t)n, primes, exps, &cnt);

    uint64_t prod = 1; /* A003961(n) mod 8 */
    for (size_t i = 0; i < cnt; ++i) {
        uint64_t np = next_prime(primes[i]) & 7u;
        for (uint64_t k = 0; k < exps[i]; ++k)
            prod = (prod * np) & 7u;
    }

    return (seq_t)(((prod + 1u) / 2u) & 3u);
}
