/*
 * A361633  a(n) is the denominator of the median of the prime factors of n
 *          with repetition.
 * https://oeis.org/A361633
 *
 * Offset:    2    (a(2) is the first term)
 * Keywords:  nonn, frac
 * Formula:   a(n) = denominator of the median of the prime factors of n (with
 *            repetition). The median of an odd-length multiset is an integer,
 *            so the denominator is 1. For an even-length multiset consisting
 *            of the two middle prime factors p and q, the median is (p+q)/2
 *            and the denominator is 1 if p+q is even,...
 * Range:     u8 n <= 89, u16 n <= 89, u32 n <= 89, u64 n <= 89, u128 n <= 89,
 *            big any n
 * Helpers:   factor_u64
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  OEIS: A361633 */

#include <stddef.h>
#include <stdint.h>
#include "../utils/factor_u64.h"

seq_t a(idx_t n) {
    if (n < 2) return (seq_t)0;   /* undefined for n < 2, return 0 */

    /* Factor n into primes (ascending) and exponents. */
    uint64_t primes[64];
    uint64_t exps[64];
    size_t cnt = 0;
    factor_u64((uint64_t)n, primes, exps, &cnt);

    /* Count total multiplicity of prime factors. */
    size_t total = 0;
    for (size_t i = 0; i < cnt; ++i) {
        total += exps[i];
    }

    /* Odd number of factors => median is an integer. */
    if (total & 1) {
        return (seq_t)1;
    }

    /* Even number of factors: find the two middle primes. */
    size_t half = total / 2;
    size_t pos = 0;
    uint64_t mid1 = 0, mid2 = 0;

    for (size_t i = 0; i < cnt; ++i) {
        uint64_t p = primes[i];
        for (uint64_t j = 0; j < exps[i]; ++j) {
            if (pos == half - 1) mid1 = p;
            if (pos == half) {
                mid2 = p;
                goto found;
            }
            ++pos;
        }
    }

found:
    /* The denominator is 1 if the sum mid1+mid2 is even, else 2.
       Check parity without overflow: parity of a+b is XOR of parities. */
    uint64_t parity = (mid1 & 1) ^ (mid2 & 1);
    return (seq_t)(parity ? 2 : 1);
}
