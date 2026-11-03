/*
 * A328260  a(n) = n * omega(n).
 * https://oeis.org/A328260
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = n * ω(n) (ω = number of distinct prime factors)
 * Range:     u8 n <= 89, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Validated: standard, pass, 86 terms through n = 9166 (2026-11-01)
 * Helpers:   omega
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>
#include "../utils/omega.h"

seq_t a(idx_t n)
{
    /* n ≥ 1 for valid OEIS terms; for n = 0 return 0 (unused) */
    if (n == 0)
        return (seq_t)0;
    int distinct = omega((uint64_t)n);
    return (seq_t)n * (seq_t)distinct;
}
