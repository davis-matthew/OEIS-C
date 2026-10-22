/*
 * A353975  Product of the divisors of n whose arithmetic derivative is odd.
 * https://oeis.org/A353975
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Product_{d|n} d^((d') mod 2)
 * Range:     u8 n <= 17, u16 n <= 41, u32 n <= 62, u64 n <= 62, u128 n <= 62,
 *            big any n
 * Helpers:   big_pow, factor_u64, seq_mul
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stddef.h>
#include <stdbool.h>

#include "../utils/factor_u64.h"
#include "../utils/big_pow.h"
#include "../utils/seq_mul.h"

seq_t a(idx_t n)
{
    /* Factorize n */
    uint64_t primes[64];
    uint64_t exps[64];
    size_t cnt;
    factor_u64((uint64_t)n, primes, exps, &cnt);

    idx_t e0 = 0;                 /* exponent of 2 in n */
    idx_t oddcnt = 0;              /* number of distinct odd primes */
    uint64_t odd_primes[64];
    idx_t odd_exps[64];

    for (size_t i = 0; i < cnt; ++i) {
        if (primes[i] == 2) {
            e0 = (idx_t)exps[i];
        } else {
            odd_primes[oddcnt] = primes[i];
            odd_exps[oddcnt] = (idx_t)exps[i];
            ++oddcnt;
        }
    }

    seq_t result = (seq_t)1;

    /* Exponent of 2 in the result */
    if (e0 >= 1) {
        idx_t exp2 = 1;
        for (size_t i = 0; i < oddcnt; ++i) {
            exp2 *= (odd_exps[i] + 1);
        }
        seq_t pow2 = big_pow((seq_t)2, (seq_t)exp2);
        result = seq_mul(result, pow2);
    }

    /* Exponents for each odd prime */
    for (size_t j = 0; j < oddcnt; ++j) {
        idx_t e_j = odd_exps[j];

        /* Contributions from divisors with f0 = 1 (only if e0 >= 1) */
        idx_t S1 = 0;
        if (e0 >= 1) {
            idx_t prod_others = 1;
            for (size_t i = 0; i < oddcnt; ++i) {
                if (i != j) prod_others *= (odd_exps[i] + 1);
            }
            S1 = (e_j * (e_j + 1) / 2) * prod_others;
        }

        /* Number of assignments for the other odd primes */
        idx_t N_others = 1;
        for (size_t i = 0; i < oddcnt; ++i) {
            if (i != j) N_others *= (odd_exps[i] + 1);
        }

        /* Determine whether all other exponents are even */
        bool all_other_even = true;
        for (size_t i = 0; i < oddcnt; ++i) {
            if (i == j) continue;
            if (odd_exps[i] % 2) { all_other_even = false; break; }
        }

        /* Count of assignments with odd number of odd exponents among the others */
        idx_t N_odd_others;
        if (all_other_even) {
            /* When all other exponents are even, the number with odd parity is (N_others - 1)/2 */
            N_odd_others = (N_others - 1) / 2;
        } else {
            /* Otherwise, it is N_others/2 */
            N_odd_others = N_others / 2;
        }
        idx_t N_even_others = N_others - N_odd_others;

        /* Sums of even and odd exponents of f_j itself */
        idx_t m = e_j / 2;
        idx_t Sum_even, Sum_odd;
        if (e_j % 2 == 0) {          /* e_j even   = 2m */
            Sum_even = m * (m + 1);  /* m(m+1)      */
            Sum_odd  = m * m;        /* m^2          */
        } else {                     /* e_j odd    = 2m+1 */
            Sum_even = m * (m + 1);          /* m(m+1)      */
            Sum_odd  = (m + 1) * (m + 1);    /* (m+1)^2      */
        }

        /* Contributions from divisors with f0 = 0 and odd number of odd exponents */
        idx_t Sum_f_j = N_odd_others * Sum_even + N_even_others * Sum_odd;

        /* Total exponent for this prime */
        idx_t exp_j = S1 + Sum_f_j;

        if (exp_j != 0) {
            seq_t pow_val = big_pow((seq_t)odd_primes[j], (seq_t)exp_j);
            result = seq_mul(result, pow_val);
        }
    }

    return result;
}
