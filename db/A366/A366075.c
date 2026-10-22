/*
 * A366075  The number of primes dividing the smallest coreful infinitary
 *          divisor of n, counted with multiplicity.
 * https://oeis.org/A366075
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = sum over prime factors p of n of the lowest set bit of
 *            the exponent e of p in n (i.e., a(p^e) = e & -e)
 * Range:     u8 n <= 10000, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   factor_u64
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  These utilities are provided by the build environment. */
#include <stdint.h>
#include <stdbool.h>
#include "../utils/factor_u64.h"

/* Computes the number of prime factors (counted with multiplicity) of the smallest coreful infinitary divisor of n. */
seq_t a(idx_t n) {
    if (n < 2)
        return 0;

    uint64_t nn = (uint64_t)n;
    /* Temporary arrays big enough for any 64‑bit number (at most 64 distinct prime factors). */
    uint64_t primes[64];
    uint64_t exps[64];
    size_t cnt = 0;

    /* factor_u64 returns the prime factorisation of nn into primes[0..cnt-1] and exps[0..cnt-1] */
    factor_u64(nn, primes, exps, &cnt);

    seq_t sum = 0;
    for (size_t i = 0; i < cnt; ++i) {
        uint64_t e = exps[i];
        uint64_t lsb = e & (~(e - 1));  /* lowest set bit of e */
        sum += (seq_t)lsb;
    }
    return sum;
}
