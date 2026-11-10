/*
 * A266407  Permutation of natural numbers: a(n) = A064989(A263273((2*n)-1)).
 * https://oeis.org/A266407
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) = A064989(A263273(2*n-1)); A263273 = bijective base-3
 *            reverse, A064989 multiplicative with a(2^e)=1,
 *            a(p^e)=prevprime(p)^e
 * Range:     u8 n <= 122, u16 n <= 9842, u32 n <= 9842, u64 n <= 9842,
 *            u128 n <= 9842, big any n
 * Validated: standard, pass, 95 terms through n = 9691 (2026-11-07)
 * Helpers:   factor_u64, is_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include <stdint.h>
#include <stdlib.h>
#include "../utils/factor_u64.h"
#include "../utils/is_prime.h"

static uint64_t A263273_val(uint64_t n)
{
    if (n == 0) return 0;
    uint64_t temp = n, pow3 = 1;
    while (temp % 3 == 0) { temp /= 3; pow3 *= 3; }
    uint64_t rev = 0, m = temp;
    while (m > 0) { rev = rev * 3 + m % 3; m /= 3; }
    return rev * pow3;
}

static uint64_t A064989_prev_prime(uint64_t n)
{
    uint64_t p = n - 1;
    for (;; --p) {
        if (is_prime((idx_t)p)) return p;
    }
}

static seq_t A064989_val(uint64_t n)
{
    uint64_t pr[64];
    uint64_t ex[64];
    size_t cnt = 0;
    factor_u64(n, pr, ex, &cnt);
    seq_t result = 1;
    for (size_t i = 0; i < cnt; ++i) {
        if (pr[i] == 2) continue;
        seq_t base = (seq_t)A064989_prev_prime(pr[i]);
        for (uint64_t k = 0; k < ex[i]; ++k) result *= base;
    }
    return result;
}

seq_t a(idx_t n)
{
    if (n < 1) return 0;
    uint64_t m = A263273_val((uint64_t)(2 * n - 1));
    return A064989_val(m);
}
