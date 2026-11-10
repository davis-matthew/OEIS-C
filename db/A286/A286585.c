/*
 * A286585  a(n) = A053735(A048673(n)).
 * https://oeis.org/A286585
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = A053735(A048673(n)), A048673(n) = (A003961(n)+1)/2,
 *            A053735 = sum of base-3 digits
 * Range:     u8 n <= 47, u16 n <= 8191, u32 n <= 8192, u64 n <= 8192,
 *            u128 n <= 8192, big any n
 * Validated: standard, pass, 120 terms through n = 7089 (2026-11-08)
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

static seq_t A053735_a(seq_t n)
{
    seq_t sum = 0;
    for (seq_t x = n; x != 0; x /= 3)
        sum += x % 3;
    return sum;
}

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
        uint64_t np = next_prime(primes[i]);
        for (uint64_t k = 0; k < exps[i]; ++k)
            prod = prod * (seq_t)np;
    }
    return (prod + (seq_t)1) / (seq_t)2;
}

seq_t a(idx_t n)
{
    return A053735_a(A048673_a(n));
}
