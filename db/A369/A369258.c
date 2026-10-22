/*
 * A369258  a(n) = number of odd divisors of n that have an odd number of
 *          prime factors with multiplicity.
 * https://oeis.org/A369258
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = number of odd divisors of n that have an odd number of
 *            prime factors (counted with multiplicity). The implementation
 *            factors the odd part of n and counts divisors whose Ω(d) is odd
 *            by a parity DP
 * Range:     u8 n <= 9957, u16 n <= 9957, u32 n <= 9957, u64 n <= 9957,
 *            u128 n <= 9957, big any n
 * Helpers:   factor_u64
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>
#include <stddef.h>
#include "../utils/factor_u64.h"

seq_t a(idx_t n) {
    if (n < 1) return (seq_t)0;                /* out of range */

    /* Compute the odd part of n (remove all factors of 2). */
    uint64_t odd_part = (uint64_t)n;
    while ((odd_part & 1ULL) == 0) odd_part >>= 1;
    if (odd_part == 1) return (seq_t)0;        /* n is a power of two */

    /*  Factor odd_part.  factor_u64 uses up to 64 primes, enough for
        64‑bit numbers. */
    uint64_t primes[64];
    uint64_t exps[64];
    size_t cnt = 0;
    factor_u64(odd_part, primes, exps, &cnt);

    /*  DP: count divisors with even/odd Ω.  We use native 64‑bit
        arithmetic for safety; the final answer fits in seq_t. */
    unsigned long long even = 1;   /* divisors counted so far with even Ω */
    unsigned long long odd  = 0;   /* divisors counted so far with odd  Ω */

    for (size_t i = 0; i < cnt; ++i) {
        unsigned long long e   = (unsigned long long)exps[i];
        unsigned long long cnt0 = e / 2ULL + 1ULL;   /* exponents ≡ 0 (mod 2) */
        unsigned long long cnt1 = (e + 1ULL) / 2ULL; /* exponents ≡ 1 (mod 2) */

        unsigned long long new_even = even * cnt0 + odd * cnt1;
        unsigned long long new_odd  = odd  * cnt0 + even * cnt1;
        even = new_even;
        odd  = new_odd;
    }

    return (seq_t)odd;
}
