/*
 * A373481  a(n) = 1 if A003415(n) is a multiple of A001414(n), otherwise 0,
 *          where A003415 is the arithmetic derivative, and A001414 is the sum
 *          of prime factors with multiplicity.
 * https://oeis.org/A373481
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 1 if A003415(n) is a multiple of A001414(n), otherwise 0
 *            (A003415 = arithmetic derivative, A001414 = sum of prime factors
 *            with multiplicity). Special case: a(1) = 1
 * Range:     u8 n <= 1648, u16 n <= 9694, u32 n <= 9694, u64 n <= 9694,
 *            u128 n <= 9694, big any n
 * Helpers:   build_primes_upto
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>
#include "../utils/build_primes_upto.h"

static uint64_t *primes = NULL;   /* array of primes */
static size_t prime_count = 0;    /* number of primes */

/* Initialise the prime table once. */
static void init_primes(void)
{
    if (primes == NULL) {
        size_t cnt;
        /* 100000 is safely above sqrt(10000); can be larger without harm. */
        build_primes_upto(100000, &primes, &cnt);
        prime_count = cnt;
    }
}

/* Compute the OEIS sequence A373481 for index n. */
seq_t a(idx_t n)
{
    if (n == 1)
        return (seq_t)1;

    init_primes();                       /* ensure primes are ready */

    idx_t n_remaining = n;               /* working copy for factorisation */
    seq_t n_seq = (seq_t)n;              /* original n in seq_t for calculations */
    seq_t derivative = (seq_t)0;
    seq_t sumFactors = (seq_t)0;

    for (size_t i = 0; i < prime_count; ++i) {
        idx_t p = (idx_t)primes[i];
        if (p * p > n_remaining)
            break;

        if (n_remaining % p == 0) {
            int e = 0;
            while (n_remaining % p == 0) {
                n_remaining /= p;
                ++e;
            }
            /* derivative contribution: e * (n / p) */
            derivative += (seq_t)e * (n_seq / (seq_t)p);
            /* sum of prime factors contribution: e * p */
            sumFactors += (seq_t)e * (seq_t)p;
        }
    }

    /* If a prime factor larger than sqrt(n) remains. */
    if (n_remaining > 1) {
        seq_t p = (seq_t)n_remaining;          /* remaining prime */
        /* derivative contribution: 1 * (n / p) */
        derivative += n_seq / p;
        /* sum of prime factors contribution: 1 * p */
        sumFactors += p;
    }

    /* Check divisibility. */
    if (sumFactors == 0)          /* should not happen for n > 1 */
        return (seq_t)0;

    return (seq_t)((derivative % sumFactors == 0) ? 1 : 0);
}
