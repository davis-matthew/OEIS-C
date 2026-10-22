/*
 * A373475  Numbers k such that A001414(k) and A083345(k) are both multiples
 *          of 3, where A001414 is fully additive with a(p) = p, and A083345
 *          is the numerator of the fully additive function with a(p) = 1/p.
 * https://oeis.org/A373475
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = the n-th integer k such that * A001414(k) (sum of prime
 *            factors with multiplicity) is a multiple of 3 * A083345(k)
 *            (numerator of the fully‑additive function with a(p)=1/p) * is
 *            also a multiple of 3. * * The sequence starts
 *            1,8,14,20,26,35,38,44,… as given in OEIS. * * Computation:
 *            iterate...
 * Range:     u8 n <= 49, u16 n <= 9909, u32 n <= 9909, u64 n <= 9909,
 *            u128 n <= 9909, big any n
 * Helpers:   gcd
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>
#include <stddef.h>
#include "../utils/gcd.h"   /* provides seq_t gcd(seq_t, seq_t) */

/* factorise n into distinct primes and their exponents
 * primes[] and exps[] must be allocated with large enough space
 * *cnt gets set to the number of distinct prime factors
 */
static void factorise(idx_t n, uint64_t *primes, int *exps, int *cnt)
{
    idx_t m = n;
    *cnt = 0;

    /* special case n == 1 */
    if (m <= 1)
        return;

    /* factor 2 */
    if (m % 2 == 0) {
        int e = 0;
        while (m % 2 == 0) {
            m /= 2;
            ++e;
        }
        primes[(*cnt)++] = 2;
        exps[ *cnt - 1 ] = e;
    }

    /* odd factors */
    for (idx_t p = 3; p * p <= m; p += 2) {
        if (m % p == 0) {
            int e = 0;
            while (m % p == 0) {
                m /= p;
                ++e;
            }
            primes[(*cnt)++] = p;
            exps[ *cnt - 1 ] = e;
        }
    }
    /* remaining prime > sqrt(original n) */
    if (m > 1) {
        primes[(*cnt)++] = m;
        exps[ *cnt - 1 ] = 1;
    }
}

seq_t a(idx_t n)
{
    idx_t found = 0;
    idx_t k = 1;

    /* temporary arrays for factorisation (max 20 distinct primes for k ≤ 1e9) */
    uint64_t primes[20];
    int exps[20];
    int pc;

    while (found < n) {
        /* factorise current k */
        factorise(k, primes, exps, &pc);

        /* compute A001414(k): sum(p * e) */
        seq_t sopfr = 0;
        for (int i = 0; i < pc; ++i)
            sopfr += (seq_t)primes[i] * (seq_t)exps[i];

        /* compute A083345(k) numerator after reduction */
        seq_t D = 1;
        for (int i = 0; i < pc; ++i)
            D *= (seq_t)primes[i];

        seq_t N = 0;
        for (int i = 0; i < pc; ++i) {
            seq_t term = D / (seq_t)primes[i];
            term *= (seq_t)exps[i];
            N += term;
        }

        seq_t g = gcd(N, D);
        seq_t reduced_N = N / g;

        /* test the two divisibility conditions */
        if ((sopfr % 3 == 0) && (reduced_N % 3 == 0))
            ++found;

        ++k;
    }

    return (seq_t)(k - 1);
}
