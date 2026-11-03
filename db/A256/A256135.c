/*
 * A256135  a(n) = 5^A000120(n).
 * https://oeis.org/A256135
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 5^A000120(n) – 5 raised to the Hamming weight (popcount)
 *            of n
 * Range:     u8 n <= 14, u16 n <= 63, u32 n <= 63, u64 n <= 63, u128 n <= 63,
 *            big any n
 * Validated: partial, pass, 64 terms through n = 63 (2026-11-01)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>

static inline seq_t power5(idx_t exp)
{
    seq_t result = 1;
    seq_t base   = 5;
    while (exp)
    {
        if (exp & 1)
            result *= base;
        base *= base;
        exp >>= 1;
    }
    return result;
}

static inline idx_t popcnt(idx_t x) {
#if defined(__GNUC__) || defined(__clang__)
    /* GCC/Clang built‑in on unsigned long long */
    return __builtin_popcountll(x);
#else
    /* Portable fallback: Kernighan’s algorithm */
    idx_t count = 0;
    while (x) {
        x &= x - 1;
        ++count;
    }
    return count;
#endif
}

seq_t a(idx_t n) {
    return power5(popcnt(n));
}
