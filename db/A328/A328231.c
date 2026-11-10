/*
 * A328231  a(n) = gcd(n, A048673(n)).
 * https://oeis.org/A328231
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = gcd(n, A048673(n)), A048673(n) = (A003961(n)+1)/2
 * Range:     u8 n <= 47, u16 n <= 4159, u32 n <= 65537, u64 n <= 65537,
 *            u128 n <= 65537, big any n
 * Validated: standard, pass, 121 terms through n = 8673 (2026-11-08)
 * Helpers:   factor_u64, gcd, next_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stddef.h>
#include <stdbool.h>
#include <stdlib.h>
#include "../utils/factor_u64.h"
#include "../utils/next_prime.h"
#include "../utils/gcd.h"

static seq_t A048673_a(idx_t n)
{
    if (n == 1)
        return (seq_t)1;

    uint64_t primes[64];
    uint64_t exps[64];
    size_t cnt = 0;

    factor_u64((uint64_t)n, primes, exps, &cnt);

    seq_t prod = (seq_t)1;
    for (size_t i = 0; i < cnt; ++i) {
        uint64_t p = primes[i];
        uint64_t e = exps[i];
        uint64_t np = next_prime(p);
        for (uint64_t k = 0; k < e; ++k)
            prod = prod * (seq_t)np;
    }
    return (prod + (seq_t)1) / (seq_t)2;
}

seq_t a(idx_t n)
{
    return gcd((seq_t)n, A048673_a(n));
}
