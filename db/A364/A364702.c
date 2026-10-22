/*
 * A364702  Numbers k in A361098 that are not divisible by A007947(k)^2.
 * https://oeis.org/A364702
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 20, u16 n <= 8050, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   append_candidate, factor_u64, is_prime_small
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* 
   This function computes the OEIS sequence A364702.
   It generates the numbers k that belong to A361098 but are not in A286708.
   Conditions for a number k:
      • k is not a prime power and not square‑free (Ω(k) > ω(k) > 1).
      • Let r = rad(k) be the product of distinct primes dividing k.
      • Let p be the second smallest prime factor of k (the second distinct prime dividing k).
      • Let q be the smallest prime that does not divide k.
      • k satisfies q·r < k   and   p·r ≤ k.
      • k is NOT divisible by r² (i.e., at least one exponent equals 1).
   The implementation iterates over integers, tests these conditions
   via prime factorisation (using the helper factor_u64) and builds
   the sequence on demand, caching all produced terms.
*/

#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>
#include <stdlib.h>
#include "../utils/factor_u64.h"
#include "../utils/is_prime_small.h"
#include "../utils/append_candidate.h"

seq_t a(idx_t n)
{
    static seq_t *terms = NULL;          /* cached sequence terms   */
    static size_t seq_len = 0;           /* number of terms stored  */
    static size_t seq_cap = 0;           /* capacity of the array   */
    static idx_t current = 1;            /* next integer to test    */

    /* Keep generating until we have at least n terms */
    while (seq_len < n) {
        ++current;                            /* test next integer           */
        uint64_t primes[64];
        uint64_t exps[64];
        size_t cnt;

        factor_u64((uint64_t)current, primes, exps, &cnt);

        /* Need at least two distinct primes (ω(k) > 1) */
        if (cnt < 2)
            continue;

        /* Compute Ω(k) = sum of exponents and check Ω > ω */
        seq_t omega = 0;
        for (size_t i = 0; i < cnt; ++i)
            omega += (seq_t)exps[i];
        if (omega <= (seq_t)cnt)            /* square‑free, skip         */
            continue;

        /* Ensure that NOT all exponents are ≥ 2 (k ∉ T) */
        bool any_exp_eq1 = false;
        for (size_t i = 0; i < cnt; ++i)
            if (exps[i] == 1)
                { any_exp_eq1 = true; break; }
        if (!any_exp_eq1)
            continue;                       /* all exponents ≥ 2         */

        /* Compute r = rad(k) = product of distinct primes */
        seq_t rad = 1;
        for (size_t i = 0; i < cnt; ++i)
            rad *= (seq_t)primes[i];

        /* Second smallest distinct prime factor p = primes[1] */
        seq_t p = (seq_t)primes[1];

        /* Find smallest prime q that does NOT divide current */
        uint64_t q = 0;
        for (uint64_t pr = 2; ; ++pr) {
            if (!is_prime_small(pr))
                continue;
            if (current % pr != 0) {
                q = pr;
                break;
            }
        }

        /* Apply inequality tests */
        if ((seq_t)q * rad >= (seq_t)current)   /* q·rad < k   */
            continue;
        if ((seq_t)p * rad >  (seq_t)current)   /* p·rad ≤ k   */
            continue;

        /* All conditions satisfied – append to cached sequence */
        append_candidate(&terms, &seq_len, &seq_cap, (seq_t)current);
    }

    return terms[n - 1];
}
