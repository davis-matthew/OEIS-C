/*
 * A381605  Number of distinct prime divisors of n^3+1.
 * https://oeis.org/A381605
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = number of distinct prime divisors of n^3 + 1
 * Range:     u8 n <= 10000, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Validated: standard, pass, 116 terms through n = 9341 (2026-11-01)
 * Helpers:   omega
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>
#include "../utils/omega.h"

/* Compute a(n) for n ≥ 1 */
seq_t a(idx_t n)
{
    /* n is an unsigned 64‑bit integer.  Compute n^3 + 1 in 64‑bit
       arithmetic; for the test ranges this never overflows. */
    uint64_t v = (uint64_t)n;
    v = v * v * v + 1;

    /* Count distinct prime factors of the 64‑bit value. */
    int cnt = omega(v);

    return (seq_t)cnt;
}
