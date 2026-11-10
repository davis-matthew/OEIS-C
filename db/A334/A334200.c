/*
 * A334200  Fully additive with a(n) = n-1 for n <= 3, and a(p) = 1 +
 *          a(A048673(p)) when p is prime > 3 and a(n*m) = a(n) + a(m) when
 *          m,n > 1.
 * https://oeis.org/A334200
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) fully additive; a(1)=0, a(2)=1, a(3)=2, a(p) = 1 +
 *            a((nextprime(p)+1)/2) for primes p > 3 [A048673(p) =
 *            (nextprime(p)+1)/2]
 * Range:     u8 n <= 65537, u16 n <= 65537, u32 n <= 65537, u64 n <= 65537,
 *            u128 n <= 65537, big any n
 * Validated: standard, pass, 121 terms through n = 9781 (2026-11-08)
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

static seq_t A334200_f(uint64_t n)
{
    if (n <= 3)
        return (seq_t)(n - 1);

    uint64_t primes[64];
    uint64_t exps[64];
    size_t cnt = 0;
    factor_u64(n, primes, exps, &cnt);

    seq_t total = (seq_t)0;
    for (size_t i = 0; i < cnt; ++i) {
        uint64_t p = primes[i];
        uint64_t e = exps[i];
        seq_t v;
        if (p <= 3)
            v = (seq_t)(p - 1);
        else
            v = (seq_t)1 + A334200_f((next_prime(p) + 1) / 2);
        for (uint64_t k = 0; k < e; ++k)
            total += v;
    }
    return total;
}

seq_t a(idx_t n)
{
    return A334200_f((uint64_t)n);
}
