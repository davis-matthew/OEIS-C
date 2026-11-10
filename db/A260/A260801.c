/*
 * A260801  Primes p such that A008908(p) is also prime.
 * https://oeis.org/A260801
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = n-th prime p such that A008908(p) (Collatz steps to 1,
 *            plus 1) is also prime; found by a search over primes
 * Range:     u8 n <= 15, u16 n <= 58, u32 n <= 58, u64 n <= 58, u128 n <= 58,
 *            big any n
 * Validated: partial, pass, 58 terms through n = 58 (2026-11-07)
 * Helpers:   is_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>
#include "../utils/is_prime.h"

static idx_t A008908_steps(idx_t n)
{
    unsigned __int128 cur = (unsigned __int128)n;
    idx_t steps = 0;
    while (cur != 1) {
        if ((cur & 1) == 0) cur >>= 1;
        else cur = cur * 3 + 1;
        ++steps;
        if (steps > 2000000) return 0;
    }
    return steps + 1;
}

seq_t a(idx_t n)
{
    idx_t count = 0;
    idx_t p = 1;
    while (count < n) {
        ++p;
        if (!is_prime(p)) continue;
        idx_t v = A008908_steps(p);
        if (v >= 2 && is_prime(v)) ++count;
    }
    return (seq_t)p;
}
