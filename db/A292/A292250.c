/*
 * A292250  Binary encoding of 0-digits in ternary representation of
 *          A048673(n).
 * https://oeis.org/A292250
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) = A291770(A048673(n)); A048673(n) = (A003961(n)+1)/2,
 *            A291770(m) = binary word with 1s at the non-leading zero digits
 *            of m in base 3
 * Range:     u8 n <= 1279, u16 n <= 16384, u32 n <= 16384, u64 n <= 16384,
 *            u128 n <= 16384, big any n
 * Validated: standard, pass, 132 terms through n = 9401 (2026-11-08)
 * Helpers:   factor_u64, next_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stddef.h>
#include <stdbool.h>
#include "../utils/factor_u64.h"
#include "../utils/next_prime.h"

static uint64_t A048673_val(uint64_t n)
{
    if (n <= 1) return 1;
    uint64_t primes[64];
    uint64_t exps[64];
    size_t cnt = 0;
    factor_u64(n, primes, exps, &cnt);
    uint64_t prod = 1;
    for (size_t i = 0; i < cnt; ++i) {
        uint64_t np = next_prime(primes[i]);
        for (uint64_t k = 0; k < exps[i]; ++k)
            prod *= np;
    }
    return (prod + 1) / 2;
}

seq_t a(idx_t n)
{
    uint64_t m = A048673_val((uint64_t)n);
    uint8_t digits[64];
    int len = 0;
    while (m > 0) {
        digits[len++] = (uint8_t)(m % 3);
        m /= 3;
    }
    seq_t result = 0;
    bool seen = false;
    for (int i = len - 1; i >= 0; --i) {
        if (digits[i] != 0) seen = true;
        if (seen) {
            result = result * 2 + (seq_t)(digits[i] == 0 ? 1 : 0);
        }
    }
    return result;
}
