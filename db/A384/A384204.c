/*
 * A384204  a(n) is the number of palindromic permutations of the prime
 *          factors of n (counted with multiplicity).
 * https://oeis.org/A384204
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = the number of palindromic permutations of the prime
 *            factors of n (with multiplicity). If n has more than one prime
 *            with an odd exponent the value is 0. Computed by factorising n,
 *            removing at most one odd exponent, and evaluating the
 *            multinomial coefficient for the remaining square–part exp...
 * Range:     u8 n <= 10000, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   factor_u64
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stddef.h>
#include "../utils/factor_u64.h"

seq_t a(idx_t n)
{
    /* a(1) = 1  (empty factorisation) */
    if (n == 1) return (seq_t)1;

    /* Factorise n */
    uint64_t primes[64];
    uint64_t exps[64];
    size_t cnt;
    factor_u64((uint64_t)n, primes, exps, &cnt);

    /* Count primes with an odd exponent; remember the index of that one */
    int odd_idx = -1;
    int odd_cnt = 0;
    for (size_t i = 0; i < cnt; ++i) {
        if (exps[i] & 1ULL) {
            ++odd_cnt;
            odd_idx = (int)i;
            if (odd_cnt > 1) return (seq_t)0;
        }
    }

    /* Exponents of the perfect–square part s (s² | n) */
    uint64_t sexps[64];
    for (size_t i = 0; i < cnt; ++i) {
        if ((int)i == odd_idx)
            sexps[i] = (exps[i] - 1ULL) >> 1;
        else
            sexps[i] = exps[i] >> 1;
    }

    /* Compute (∑ e_i)! / ∏ e_i!  for the square part */
    idx_t total = 0;
    seq_t res = 1;

    for (size_t i = 0; i < cnt; ++i) {
        uint64_t e = sexps[i];
        for (uint64_t j = 1; j <= e; ++j) {
            /* use 128‑bit intermediate to avoid overflow */
            unsigned __int128 prod = (unsigned __int128)res * (total + j);
            res = prod / j;            /* exact division */
        }
        total += e;
    }

    return res;
}
