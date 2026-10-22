/*
 * A322035  Let p1 <= p2 <= ... <= pk be the prime factors of n, with
 *          repetition; let s = 1/p1 + 1/(p1*p2) + 1/(p1*p2*p3) + ... +
 *          1/(p1*p2*...*pk); a(n) = denominator of s. a(1)=1 by convention.
 * https://oeis.org/A322035
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, frac
 * Formula:   a(n) is the reduced denominator of
 * Range:     u8 n <= 255, u16 n <= 9895, u32 n <= 9895, u64 n <= 9895,
 *            u128 n <= 9895, big any n
 * Helpers:   factor_u64, gcd
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* s = 1/p1 + 1/(p1*p2) + ... + 1/(p1*...*pk) */
/* where p1 <= ... <= pk are the prime factors of n (with multiplicity). */
#include <stdint.h>
#include <stdbool.h>
#include "../utils/factor_u64.h"
#include "../utils/gcd.h"

seq_t a(idx_t n) {
    if (n <= 1) {
        return (seq_t)1;
    }
    uint64_t u = (uint64_t)n;

    /* factor_u64 fills primes[] and exps[] with ascending primes and their multiplicities */
    uint64_t primes[64];
    uint64_t exps[64];
    size_t cnt = 0;
    factor_u64(u, primes, exps, &cnt);

    /* compute D = product of all primes with multiplicities */
    seq_t denom = 1;
    for (size_t i = 0; i < cnt; ++i) {
        seq_t p = (seq_t)primes[i];
        for (uint64_t e = 0; e < exps[i]; ++e) {
            denom *= p;
        }
    }

    /* compute N = sum_{i=1..k} product_{j=i+1..k} p_j */
    seq_t numer = 0;
    seq_t suffix = 1;  /* product of primes processed so far, starting with empty set */
    for (int i = (int)cnt - 1; i >= 0; --i) {
        seq_t p = (seq_t)primes[i];
        for (uint64_t e = 0; e < exps[i]; ++e) {
            numer += suffix;
            suffix *= p;
        }
    }

    /* reduced denominator = denom / gcd(numer, denom) */
    seq_t g = gcd(numer, denom);
    return denom / g;
}
