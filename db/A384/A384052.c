/*
 * A384052  The number of integers k from 1 to n such that the greatest
 *          divisor of k that is a unitary divisor of n is a square.
 * https://oeis.org/A384052
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy, mult
 * Formula:   a(n) = n - Σ_{∅≠S⊆O} (-1)^{|S|+1} ⌊n / ∏_{p∈S}p^e⌋ where O is
 *            the set of prime divisors of n with an odd exponent e
 * Range:     u8 n <= 255, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>

/* Computes p^ex for uint64_t base p and integer exponent ex. */
static seq_t pow_uint64(uint64_t p, int ex) {
    seq_t result = 1;
    for (int i = 0; i < ex; ++i) result *= (seq_t)p;
    return result;
}

/* Main sequence function */
seq_t a(idx_t n) {
    if (n == 0) return 0;   /* undefined for n<1, but return 0 for safety */

    uint64_t tmp = (uint64_t)n;
    /* Arrays holding odd-exponent primes and their full prime power */
    uint64_t primes[64];
    seq_t  values[64];
    int odd_cnt = 0;

    for (uint64_t p = 2; p * p <= tmp; ++p) {
        if (tmp % p != 0) continue;
        int exp = 0;
        while (tmp % p == 0) {
            tmp /= p;
            ++exp;
        }
        if (exp & 1) {   /* odd exponent */
            primes[odd_cnt]   = p;
            values[odd_cnt]   = pow_uint64(p, exp);
            ++odd_cnt;
        }
    }
    if (tmp > 1) {   /* remaining prime factor has exponent 1 (odd) */
        primes[odd_cnt]   = tmp;
        values[odd_cnt]   = (seq_t)tmp;   /* p^1 */
        ++odd_cnt;
    }

    if (odd_cnt == 0) {
        return (seq_t)n;   /* no odd exponents → all k satisfy condition */
    }

    /* Inclusion–exclusion over subsets of odd primes */
    seq_t removed = 0;
    /* Iterate over all non‑empty subsets via bitmask */
    for (uint32_t mask = 1; mask < (1u << odd_cnt); ++mask) {
        seq_t prod = 1;
        /* Build product of p^e for primes in this subset */
        for (int i = 0; i < odd_cnt; ++i) {
            if (mask & (1u << i)) {
                prod *= values[i];
            }
        }
        seq_t count = (seq_t)n / prod;
        if (__builtin_popcount(mask) & 1) {
            removed += count;          /* add for odd size */
        } else {
            removed -= count;          /* subtract for even size */
        }
    }

    return (seq_t)n - removed;
}
