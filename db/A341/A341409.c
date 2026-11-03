/*
 * A341409  a(n) = (Sum_{k=1..3} k^n) mod n.
 * https://oeis.org/A341409
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = (1^n + 2^n + 3^n) mod n
 * Range:     u8 n <= 271, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Validated: standard, pass, 101 terms through n = 9983 (2026-11-01)
 * Helpers:   powmod
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include "../utils/powmod.h"

seq_t a(idx_t n)
{
    /* For n=1 the modulus would be zero, return the known value directly. */
    if (n == 1)
        return (seq_t)0;

    seq_t mod = (seq_t)n;               /* ensure correct type everywhere */
    seq_t sum = powmod((seq_t)1, (uint64_t)n, mod);
    sum += powmod((seq_t)2, (uint64_t)n, mod);
    sum += powmod((seq_t)3, (uint64_t)n, mod);
    return sum % mod;
}
