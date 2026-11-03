/*
 * A346773  a(n) = Sum_{d|n} möbius(d)^n.
 * https://oeis.org/A346773
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 1 if n==1; a(n) = 0 if n is odd; a(n) = 2^ω(n) if n is
 *            even, where ω(n) is the number of distinct prime factors of n
 * Range:     u8 n <= 20000, u16 n <= 20000, u32 n <= 20000, u64 n <= 20000,
 *            u128 n <= 20000, big any n
 * Validated: standard, pass, 119 terms through n = 9885 (2026-11-01)
 * Helpers:   factorize
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>
#include "../utils/factorize.h"

seq_t a(idx_t n)
{
    if (n == 1)               // a(1) = 1
        return (seq_t)1;

    if ((n & 1) == 1)         // odd n > 1
        return (seq_t)0;

    /* Factorize n to get the number of distinct prime factors */
    uint64_t primes[64];
    int exps[64];
    size_t distinct = factorize((uint64_t)n, primes, exps);

    /* a(n) = 2^distinct */
    seq_t result = (seq_t)1;
    result <<= distinct;      // works for built‑in and boost cpp_int types
    return result;
}
