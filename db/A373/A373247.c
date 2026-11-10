/*
 * A373247  a(n) = n mod A181819(n), where A181819(n) is the prime shadow of
 *          n.
 * https://oeis.org/A373247
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = n mod A181819(n), where A181819(n) = Product prime(e_i)
 *            for n = Product prime_i^e_i, A181819(1) = 1
 * Range:     u8 n <= 65537, u16 n <= 65537, u32 n <= 65537, u64 n <= 65537,
 *            u128 n <= 65537, big any n
 * Validated: standard, pass, 121 terms through n = 9600 (2026-11-10)
 * Helpers:   factor_u64
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>
#include <stddef.h>
#include "../utils/factor_u64.h"

static uint64_t A373247_kth_prime(uint64_t k)
{
    uint64_t c = 1;
    uint64_t cnt = 0;
    while (cnt < k) {
        c++;
        bool ok = true;
        for (uint64_t d = 2; d * d <= c; d++) {
            if (c % d == 0) { ok = false; break; }
        }
        if (ok) cnt++;
    }
    return c;
}

seq_t a(idx_t n)
{
    if (n == 1)
        return (seq_t)0;

    uint64_t primes[64];
    uint64_t exps[64];
    size_t cnt = 0;
    factor_u64((uint64_t)n, primes, exps, &cnt);

    uint64_t shadow = 1;
    for (size_t i = 0; i < cnt; ++i)
        shadow *= A373247_kth_prime(exps[i]);

    return (seq_t)((uint64_t)n % shadow);
}
