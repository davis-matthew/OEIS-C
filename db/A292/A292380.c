/*
 * A292380  Base-2 expansion of a(n) encodes the steps where multiples of 4
 *          are encountered when map x -> A252463(x) is iterated down to 1,
 *          starting from x=n.
 * https://oeis.org/A292380
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   iterate x -> A252463(x) from x = n down to 1; a(n) = Sum_{i>=0}
 *            [x_i == 0 (mod 4)] * 2^i, where x_0 = n and x_{i+1} =
 *            A252463(x_i), A252463(2k) = k, A252463(odd) = A064989(odd)
 * Range:     u8 n <= 528, u16 n <= 3480, u32 n <= 16384, u64 n <= 16384,
 *            u128 n <= 16384, big any n
 * Validated: standard, pass, 121 terms through n = 9000 (2026-11-08)
 * Helpers:   is_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>
#include "../utils/is_prime.h"

static uint64_t A292380_prev_prime(uint64_t p)
{
    uint64_t q = p - 1;
    while (q > 2 && !is_prime((idx_t)q)) q--;
    return q;
}

static uint64_t A292380_A252463(uint64_t x)
{
    uint64_t m, r, p;
    if (x <= 1) return 1;
    if ((x & 1u) == 0) return x >> 1;
    m = x;
    r = 1;
    for (p = 3; p <= m / p; p += 2)
    {
        if (m % p == 0)
        {
            uint64_t q = A292380_prev_prime(p);
            while (m % p == 0)
            {
                m /= p;
                r *= q;
            }
        }
    }
    if (m > 1) r *= A292380_prev_prime(m);
    return r;
}

seq_t a(idx_t n)
{
    uint64_t x = (uint64_t)n;
    seq_t r = 0;
    seq_t bit = 1;
    while (x > 1)
    {
        if ((x & 3u) == 0) r += bit;
        bit = bit * 2;
        x = A292380_A252463(x);
    }
    return r;
}
