/*
 * A304759  Binary encoding of 1-digits in ternary representation of
 *          A048673(n).
 * https://oeis.org/A304759
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, look, base
 * Formula:   a(n) = A289813(A048673(n)); A048673(n) = (A003961(n)+1)/2;
 *            A289813(0)=0, A289813(3k)=2*A289813(k),
 *            A289813(3k+1)=2*A289813(k)+1, A289813(3k+2)=2*A289813(k)
 * Range:     u8 n <= 47, u16 n <= 5199, u32 n <= 16383, u64 n <= 16383,
 *            u128 n <= 16383, big any n
 * Validated: standard, pass, 110 terms through n = 8674 (2026-11-08)
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

static seq_t A289813_recur(seq_t x)
{
    if (x == 0)
        return 0;
    seq_t r = A289813_recur(x / 3);
    if (x % 3 == 1)
        return (r << 1) | 1;
    return r << 1;
}

seq_t a(idx_t n)
{
    return A289813_recur(A048673_a(n));
}
