/*
 * A374255  Sum of prime factors of n (with multiplicity) minus the greatest
 *          possible sum of run-compression of a permutation of the prime
 *          factors of n.
 * https://oeis.org/A374255
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = sum_{p^e || n} p*(e - min(e, (nf - e + 1)))
 * Range:     u8 n <= 87, u16 n <= 87, u32 n <= 87, u64 n <= 87, u128 n <= 87,
 *            big any n
 * Helpers:   smallest_prime_factor
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* where nf = total number of prime factors of n (with multiplicity).  */

#include <stdint.h>
#include <stddef.h>
#include "../utils/smallest_prime_factor.h"

seq_t a(idx_t n)
{
    if (n == 1) {
        return 0;              /* A374255(1) = 0 */
    }

    /* Maximum distinct primes that can occur in a 64‑bit integer is < 64. */
    uint64_t primes[64];
    int      exps[64];
    int      pcnt = 0;          /* number of distinct prime factors */

    uint64_t tmp = (uint64_t)n;
    int      total_factors = 0; /* nf */

    /* Factor n using the smallest‑prime‑factor helper */
    while (tmp > 1) {
        uint64_t p = smallest_prime_factor(tmp);
        int      e = 0;
        while ((tmp % p) == 0) {
            tmp /= p;
            ++e;
            ++total_factors;
        }
        primes[pcnt] = p;
        exps[pcnt]   = e;
        ++pcnt;
    }

    seq_t result = 0;

    for (int i = 0; i < pcnt; ++i) {
        seq_t prime = (seq_t)primes[i];
        seq_t e     = (seq_t)exps[i];

        /* other = nf - e + 1   (number of prime factors other than p, plus one) */
        seq_t other = (seq_t)total_factors - e + 1;

        /* cnt = min(e, other)   how many copies of p can be counted in the run‑compression */
        seq_t cnt = e < other ? e : other;

        /* contribution: p*(e - cnt) */
        result += prime * (e - cnt);
    }

    return result;
}
