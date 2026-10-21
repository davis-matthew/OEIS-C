/*
 * A261087  Primes p for which A155043(p) < A155043(prevprime(p)), where
 *          A155043 gives the number of steps needed to reach zero when
 *          repeatedly applying the map that replaces k with k - A000005(k).
 * https://oeis.org/A261087
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = the n-th prime p such that A155043(p) <
 *            A155043(prevprime(p)), where A155043(k) is the number of steps
 *            to reach 0 by repeatedly replacing k with k - τ(k) (τ = number
 *            of divisors). Corresponds to OEIS A261087
 * Range:     u8 n <= 3, u16 n <= 786, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>

static const size_t MAX_LIMIT = 2000000;          /* sieve upper bound */

static bool initialized = false;
static seq_t *steps          = NULL;  /* steps[0..MAX_LIMIT]             */
static uint16_t *tau         = NULL;  /* τ[0..MAX_LIMIT]                 */
static seq_t *good_primes    = NULL;  /* array of qualifying primes       */
static size_t good_count     = 0;      /* number of stored good primes     */

static void init(void)
{
    if (initialized)
        return;

    /* Allocate arrays */
    steps       = (seq_t *)malloc((MAX_LIMIT + 1) * sizeof(seq_t));
    tau         = (uint16_t *)malloc((MAX_LIMIT + 1) * sizeof(uint16_t));
    good_primes = (seq_t *)malloc((MAX_LIMIT + 1) * sizeof(seq_t)); /* generous */

    /* Smallest prime factor sieve (spf) */
    int *spf = (int *)malloc((MAX_LIMIT + 1) * sizeof(int));
    for (size_t i = 0; i <= MAX_LIMIT; ++i)
        spf[i] = 0;
    for (size_t i = 2; i <= MAX_LIMIT; ++i) {
        if (spf[i] == 0) {                 /* i is prime */
            spf[i] = (int)i;
            if (i * i <= MAX_LIMIT) {
                for (size_t j = i * i; j <= MAX_LIMIT; j += i)
                    if (spf[j] == 0)
                        spf[j] = (int)i;
            }
        }
    }

    /* τ(1)=1 */
    tau[1] = 1;
    /* compute τ(k) for k = 2..MAX_LIMIT */
    for (size_t i = 2; i <= MAX_LIMIT; ++i) {
        int p = spf[i];
        size_t m = i / p;
        unsigned exp = 1;
        while (m % p == 0) {
            m /= p;
            ++exp;
        }
        tau[i] = tau[m] * (exp + 1);
    }

    /* compute steps(k) = 1 + steps(k - τ(k)) */
    steps[0] = 0;
    for (size_t i = 1; i <= MAX_LIMIT; ++i) {
        size_t j = i - tau[i];
        steps[i] = steps[j] + 1;
    }

    /* Find qualifying primes */
    size_t prev_prime = 0;
    good_count = 0;
    for (size_t i = 2; i <= MAX_LIMIT; ++i) {
        if (spf[i] == (int)i) {                        /* i is prime */
            if (prev_prime != 0) {
                if (steps[i] < steps[prev_prime]) {
                    good_primes[good_count++] = (seq_t)i;
                }
            }
            prev_prime = i;
        }
    }

    /* Clean up */
    free(spf);

    initialized = true;
}

seq_t a(idx_t n)
{
    init();                                 /* ensure precomputation */

    if (n == 0 || (size_t)n > good_count)
        return 0;                           /* out of range */

    return good_primes[(size_t)(n - 1)];
}
