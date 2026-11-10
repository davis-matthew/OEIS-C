/*
 * A368994  a(n) = 1 if A342001(k) is a multiple of four, and 0 otherwise.
 * https://oeis.org/A368994
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 1 if A342001(n) == 0 (mod 4) else 0, where A342001(n) =
 *            Sum_{p^v || n} v*rad(n)/p
 * Range:     u8 n <= 65537, u16 n <= 65537, u32 n <= 65537, u64 n <= 65537,
 *            u128 n <= 65537, big any n
 * Validated: standard, pass, 137 terms through n = 9647 (2026-11-10)
 * Helpers:   factor_u64
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stddef.h>
#include "../utils/factor_u64.h"

seq_t a(idx_t n)
{
    if (n <= 1)
        return (seq_t)1;

    uint64_t primes[64];
    uint64_t exps[64];
    size_t cnt = 0;
    factor_u64((uint64_t)n, primes, exps, &cnt);

    uint64_t rad = 1;
    for (size_t i = 0; i < cnt; ++i)
        rad *= primes[i];

    uint64_t sum = 0;
    for (size_t i = 0; i < cnt; ++i) {
        uint64_t term = rad / primes[i];
        sum = (sum + (term & 3) * (exps[i] & 3)) & 3;
    }
    return (seq_t)(sum == 0 ? 1 : 0);
}
