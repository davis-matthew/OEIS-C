/*
 * A325643  a(1) = 1; for n > 1, a(n) is the least divisor d > 1 of n such
 *          that A048720(d,k) = n for some k.
 * https://oeis.org/A325643
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 256, u16 n <= 9710, u32 n <= 9710, u64 n <= 9710,
 *            u128 n <= 9710, big any n
 * Helpers:   factor_u64, poly_divisible
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A325643.  For n>1, a(n) is the smallest divisor d>1 of n
 * such that the binary polynomial of d divides the binary polynomial of n
 * over GF(2).  For even n, a(n)=2.  For odd n we test all odd divisors
 * sorted ascending until the divisibility test succeeds.  The same
 * approach reproduces all known terms and runs well below the time limit. */

#include <stdint.h>
#include <stdbool.h>
#include <stdlib.h>

#include "../utils/factor_u64.h"
#include "../utils/poly_divisible.h"

static int cmp_uint64(const void *a, const void *b)
{
    uint64_t va = *(const uint64_t *)a;
    uint64_t vb = *(const uint64_t *)b;
    return (va < vb) ? -1 : (va > vb) ? 1 : 0;
}

seq_t a(idx_t n)
{
    if (n == 1) return (seq_t)1;
    if ((n & 1) == 0) return (seq_t)2;   /* even n → 2 */

    /* n is odd and >1 */
    uint64_t un = (uint64_t)n;
    uint64_t primes[64];
    uint64_t exps[64];
    size_t pcnt = 0;
    factor_u64(un, primes, exps, &pcnt);

    /* Generate all divisors of n */
    uint64_t divisors[256];
    size_t dcnt = 1;
    divisors[0] = 1;
    for (size_t i = 0; i < pcnt; ++i) {
        uint64_t p = primes[i];
        uint64_t exp = exps[i];
        size_t cur = dcnt;
        for (uint64_t k = 1; k <= exp; ++k) {
            uint64_t mul = 1;
            for (uint64_t t = 0; t < k; ++t) mul *= p;
            for (size_t j = 0; j < cur; ++j)
                divisors[dcnt++] = divisors[j] * mul;
        }
    }

    qsort(divisors, dcnt, sizeof(uint64_t), cmp_uint64);

    /* Scan divisors >1 for polynomial divisibility */
    for (size_t i = 1; i < dcnt; ++i) {
        uint64_t d = divisors[i];
        if (poly_divisible(un, d)) return (seq_t)d;
    }

    /* Fallback: return n itself (should never reach here on odd n>1) */
    return (seq_t)n;
}
