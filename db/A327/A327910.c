/*
 * A327910  This is the reduced A317745, with primes -> 1 and prime + prime ->
 *          2.
 * https://oeis.org/A327910
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   read the array T(r,c) = A101264(r) + A101264(c), r,c >= 0, by
 *            antidiagonals; A101264(k) = 1 if 2k+1 is prime, else 0
 * Range:     u8 n <= 78, u16 n <= 78, u32 n <= 78, u64 n <= 78, u128 n <= 78,
 *            big any n
 * Validated: partial, pass, 78 terms through n = 78 (2026-11-08)
 * Helpers:   is_prime_u64
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>
#include <stddef.h>
#include <math.h>
#include "../utils/is_prime_u64.h"

static seq_t A101264_a(uint64_t k)
{
    return is_prime_u64(k * 2ULL + 1ULL) ? (seq_t)1 : (seq_t)0;
}

seq_t a(idx_t n)
{
    uint64_t m = (uint64_t)n - 1ULL;
    uint64_t d = (uint64_t)((sqrt(8.0 * (double)m + 1.0) - 1.0) / 2.0);
    while ((unsigned __int128)d * (d + 1) / 2 > m) d--;
    while ((unsigned __int128)(d + 1) * (d + 2) / 2 <= m) d++;
    uint64_t k = m - (uint64_t)((unsigned __int128)d * (d + 1) / 2);
    return A101264_a(k) + A101264_a(d - k);
}
