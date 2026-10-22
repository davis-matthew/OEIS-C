/*
 * A374471  a(n) = 1 if bigomega(n) and A064547(n) are both even, otherwise 0,
 *          where A064547 is the count of 1-bits in the exponents of the prime
 *          factorization of n, and bigomega is the number of prime factors of
 *          n (with multiplicity).
 * https://oeis.org/A374471
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 1 if bigomega(n) and A064547(n) are both even, else 0
 * Range:     u8 n <= 9489, u16 n <= 9489, u32 n <= 9489, u64 n <= 9489,
 *            u128 n <= 9489, big any n
 * Helpers:   bigomega, factor_u64
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include <stdint.h>
#include <stddef.h>
#include "../utils/factor_u64.h"
#include "../utils/bigomega.h"

seq_t a(idx_t n) {
    /* Condition 1: bigomega(n) even */
    seq_t omega = bigomega(n);
    bool omega_even = (omega & 1) == 0;

    /* Condition 2: number of 1‑bits in the exponents of n's prime factorisation even */
    uint64_t m = (uint64_t)n;
    uint64_t primes[64];
    uint64_t exps[64];
    size_t cnt = 0;
    factor_u64(m, primes, exps, &cnt);

    uint64_t bitcnt = 0;
    for (size_t i = 0; i < cnt; ++i) {
        bitcnt += __builtin_popcountll(exps[i]);
    }
    bool bitcnt_even = (bitcnt & 1) == 0;

    return (omega_even && bitcnt_even) ? (seq_t)1 : (seq_t)0;
}
