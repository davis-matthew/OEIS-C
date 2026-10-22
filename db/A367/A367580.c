/*
 * A367580  Multiset multiplicity kernel (MMK) of n. Product of (least prime
 *          factor with exponent k)^(number of prime factors with exponent k)
 *          over all distinct exponents k appearing in the prime factorization
 *          of n.
 * https://oeis.org/A367580
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = product over all distinct exponents k in the prime *
 *            factorisation of n of (least prime factor with exponent
 *            k)^(number * of primes with that exponent). This implements the
 *            definition of * the Multiset multiplicity kernel (MMK)
 * Range:     u8 n <= 76, u16 n <= 76, u32 n <= 76, u64 n <= 76, u128 n <= 76,
 *            big any n
 * Helpers:   factor_u64
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stddef.h>
#include "../utils/factor_u64.h"

/* seq_t and idx_t are defined by the build environment. */
seq_t a(idx_t n)
{
    /* a(1) = 1 by definition. */
    if (n == (idx_t)1)
        return (seq_t)1;

    /* Factorise n.  factor_u64 fills primes[] and exps[] with at most
     * 64 pairs and writes the number of pairs into cnt. */
    uint64_t primes[64];
    uint64_t exps[64];
    size_t cnt = 0;
    factor_u64((uint64_t)n, primes, exps, &cnt);

    /* Group prime factors by exponent.  For each group we keep the
     * smallest prime and a counter of how many primes share that
     * exponent.  There can be at most 64 distinct exponents. */
    int exps_list[64];
    uint64_t min_prime[64];
    int group_cnt = 0;
    int count[64] = {0};

    for (size_t i = 0; i < cnt; ++i) {
        uint64_t p = primes[i];
        uint64_t e = exps[i];
        /* Search for existing group with this exponent. */
        int g;
        for (g = 0; g < group_cnt; ++g) {
            if (exps_list[g] == (int)e)
                break;
        }
        if (g < group_cnt) {          /* existing group */
            ++count[g];
            if (p < min_prime[g])
                min_prime[g] = p;
        } else {                      /* new group */
            exps_list[group_cnt] = (int)e;
            min_prime[group_cnt] = p;
            count[group_cnt] = 1;
            ++group_cnt;
        }
    }

    /* Compute the product of (min_prime[g])^(count[g]) over all groups. */
    seq_t result = (seq_t)1;
    for (int g = 0; g < group_cnt; ++g) {
        seq_t base = (seq_t)min_prime[g];
        int exp = count[g];
        seq_t power = (seq_t)1;
        for (int k = 0; k < exp; ++k)
            power *= base;
        result *= power;
    }

    return result;
}
