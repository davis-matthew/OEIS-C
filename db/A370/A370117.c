/*
 * A370117  Denominator of n/A276086(A003415(n)), where A003415 is the
 *          arithmetic derivative, and A276086 is the primorial base
 *          exp-function.
 * https://oeis.org/A370117
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, frac
 * Formula:   a(n) = denominator of n/A276086(A003415(n)) = Product_k
 *            prime(k+1)^max(0, d_k - v_{prime(k+1)}(n)), where d_k are the
 *            primorial-base digits of A003415(n)
 * Range:     u8 n <= 26, u16 n <= 47, u32 n <= 255, u64 n <= 3935,
 *            u128 n <= 10000, big any n
 * Validated: standard, pass, 92 terms through n = 9139 (2026-11-10)
 * Helpers:   factor_u64
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>
#include <stddef.h>
#include "../utils/factor_u64.h"

static bool a370117_small_prime(uint64_t p)
{
    if (p < 2) return false;
    for (uint64_t q = 2; q * q <= p; ++q)
        if (p % q == 0) return false;
    return true;
}

seq_t a(idx_t n)
{
    if (n < 2)
        return 1;

    uint64_t pr[64], ex[64];
    size_t cnt = 0;
    factor_u64((uint64_t)n, pr, ex, &cnt);

    /* arithmetic derivative */
    seq_t d = 0;
    for (size_t i = 0; i < cnt; ++i)
        d += ((seq_t)n / (seq_t)pr[i]) * (seq_t)ex[i];

    seq_t result = 1;
    uint64_t p = 2;
    while (d > 0) {
        uint64_t digit = (uint64_t)(d % (seq_t)p);
        d /= (seq_t)p;
        uint64_t v = 0;
        for (size_t i = 0; i < cnt; ++i)
            if (pr[i] == p) { v = ex[i]; break; }
        for (uint64_t j = v; j < digit; ++j)
            result *= (seq_t)p;
        do { ++p; } while (!a370117_small_prime(p));
    }
    return result;
}
