/*
 * A319708  a(n) = Product_{d|n, d<n} A276086(d).
 * https://oeis.org/A319708
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Product_{d|n, d<n} A276086(d), where A276086(d) =
 *            Product_k prime(k+1)^(digit_k of d in primorial base)
 * Range:     u8 n <= 11, u16 n <= 23, u32 n <= 47, u64 n <= 119,
 *            u128 n <= 335, big any n
 * Validated: standard, pass, 75 terms through n = 2220 (2026-11-08)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>

static seq_t A276086_exp(uint64_t m)
{
    static const uint32_t pr[16] = {2,3,5,7,11,13,17,19,23,29,31,37,41,43,47,53};
    seq_t result = 1;
    for (int k = 0; m > 0 && k < 16; ++k) {
        uint64_t r = m % pr[k];
        m /= pr[k];
        for (uint64_t j = 0; j < r; ++j)
            result *= (seq_t)pr[k];
    }
    return result;
}

seq_t a(idx_t n)
{
    seq_t res = 1;
    for (uint64_t i = 1; i * i <= n; ++i) {
        if (n % i == 0) {
            uint64_t c = n / i;
            if (i < n)
                res *= A276086_exp(i);
            if (c != i && c < n)
                res *= A276086_exp(c);
        }
    }
    return res;
}
