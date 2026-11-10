/*
 * A286586  a(n) = A006047(A048673(n)).
 * https://oeis.org/A286586
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = A006047(A048673(n)); A048673(n) = (A003961(n)+1)/2,
 *            A006047(m) = 2^r * 3^s where m in base 3 has r ones and s twos
 * Range:     u8 n <= 47, u16 n <= 7039, u32 n <= 8192, u64 n <= 8192,
 *            u128 n <= 8192, big any n
 * Validated: standard, pass, 111 terms through n = 8083 (2026-11-08)
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

static seq_t A006047_a(seq_t m)
{
    seq_t result = (seq_t)1;
    while (m > 0) {
        seq_t d = m % 3;
        if (d == 1)
            result = result * (seq_t)2;
        else if (d == 2)
            result = result * (seq_t)3;
        m = m / 3;
    }
    return result;
}

seq_t a(idx_t n)
{
    return A006047_a(A048673_a(n));
}
