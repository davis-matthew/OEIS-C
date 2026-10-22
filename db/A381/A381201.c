/*
 * A381201  a(n) is the product of the elements of the set of bases and
 *          exponents in the prime factorization of n.
 * https://oeis.org/A381201
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 256, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   factor_u64
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computes a(n) for the OEIS sequence A381201. */
/* A(n) is the product of all distinct bases and exponents appearing in the prime factorization of n. */
#include <stdint.h>
#include <stddef.h>
#include <stdbool.h>
#include "../utils/factor_u64.h"

seq_t a(idx_t n)
{
    /* factor_u64 returns the prime factorization of n in the arrays pr and ex. */
    uint64_t pr[64];
    uint64_t ex[64];
    size_t cnt = 0;
    factor_u64((uint64_t)n, pr, ex, &cnt);

    /* Collect distinct values from the bases and exponents. */
    uint64_t vals[128];
    size_t sz = 0;

    for (size_t i = 0; i < cnt; ++i) {
        uint64_t p = pr[i];
        uint64_t e = ex[i];

        /* Insert prime p if it is not already present. */
        bool exists = false;
        for (size_t j = 0; j < sz; ++j) {
            if (vals[j] == p) { exists = true; break; }
        }
        if (!exists) vals[sz++] = p;

        /* Insert exponent e if it is not already present. */
        exists = false;
        for (size_t j = 0; j < sz; ++j) {
            if (vals[j] == e) { exists = true; break; }
        }
        if (!exists) vals[sz++] = e;
    }

    /* Multiply all distinct values together, performing arithmetic in seq_t. */
    seq_t prod = (seq_t)1;
    for (size_t i = 0; i < sz; ++i) {
        prod *= (seq_t)vals[i];
    }

    return prod;
}
