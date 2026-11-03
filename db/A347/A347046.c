/*
 * A347046  Greatest divisor of n with exactly half as many prime factors
 *          (counting multiplicity) as n, or 1 if there are none.
 * https://oeis.org/A347046
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = greatest divisor d of n with bigomega(d)=bigomega(n)/2 or
 *            1 if bigomega(n) is odd
 * Range:     u8 n <= 513, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Validated: standard, pass, 97 terms through n = 9260 (2026-11-01)
 * Helpers:   factor_u64, ipow_u64_idx
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Include common utilities. */
#include <stdint.h>
#include <stdbool.h>
#include <stdlib.h>
#include <string.h>
#include "../utils/factor_u64.h"
#include "../utils/ipow_u64_idx.h"

/* Helper: safe multiplication for seq_t */
static inline seq_t seq_mul(seq_t a, seq_t b) {
    return a * b;
}

/* Compute A347046(n).  Sequence values are returned as seq_t. */
seq_t a(idx_t n)
{
    /* The sequence is defined for n >= 1.  For safety handle n == 0. */
    if (n == 0)
        return (seq_t)0;

    /* --- factor n ---------------------------------------------------- */
    uint64_t N = (uint64_t)n;
    uint64_t primes[64];
    uint64_t exps[64];
    size_t cnt = 0;
    factor_u64(N, primes, exps, &cnt);

    /* --- total number of prime factors (with multiplicity) ------------ */
    uint64_t omega_raw = 0;
    for (size_t i = 0; i < cnt; ++i)
        omega_raw += exps[i];

    /* If omega is odd, no divisor has exactly half the factors. */
    if ((omega_raw & 1U) != 0)
        return (seq_t)1;

    /* Target number of factors to take. */
    uint64_t target = omega_raw >> 1;  /* equivalent to omega_raw / 2 */

    /* --- sort primes (with their exponents) in decreasing order ---- */
    /* Simple insertion sort – the number of primes is tiny. */
    for (size_t i = 1; i < cnt; ++i) {
        size_t key_p = primes[i];
        uint64_t key_e = exps[i];
        size_t j = i;
        while (j > 0 && primes[j - 1] < key_p) {
            primes[j]     = primes[j - 1];
            exps[j]       = exps[j - 1];
            --j;
        }
        primes[j]     = key_p;
        exps[j]       = key_e;
    }

    /* --- build the maximal divisor ----------------------------------- */
    seq_t ans = (seq_t)1;
    uint64_t rem = target;     /* how many exponents still to allocate */

    for (size_t i = 0; i < cnt && rem > 0; ++i) {
        uint64_t use = exps[i];
        if (use > rem)
            use = rem;
        if (use > 0) {
            /* pow_uint returns seq_t. */
            seq_t term = ipow_u64_idx(primes[i], (idx_t)use);
            ans = seq_mul(ans, term);
        }
        rem -= use;
    }

    return ans;
}
