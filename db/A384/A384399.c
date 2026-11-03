/*
 * A384399  Consecutive states of the linear congruential pseudo-random number
 *          generator 41546*s mod (2^31-837) when started at s=1.
 * https://oeis.org/A384399
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(1)=1, a(n)=41546*a(n-1) mod 2147482811 for n>=2
 * Range:     u8 n <= 1, u16 n <= 2, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Validated: standard, pass, 39 terms through n = 9189 (2026-11-01)
 * Helpers:   powmod
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include "../utils/powmod.h"

static const seq_t MOD = (seq_t)2147482811ULL;
static const seq_t MULT = (seq_t)41546ULL;

seq_t a(idx_t n)
{
    if (n == 1) {
        return (seq_t)1;
    }
    return powmod(MULT, n - 1, MOD);
}
