/*
 * A336041  Number of refactorable divisors of n.
 * https://oeis.org/A336041
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = Sum_{d|n} [ tau(d) divides d ]
 * Range:     u8 n <= 65537, u16 n <= 65537, u32 n <= 65537, u64 n <= 65537,
 *            u128 n <= 65537, big any n
 * Validated: standard, pass, 108 terms through n = 9943 (2026-11-01)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  where tau(d) is the number of divisors of d.     */

#include <stddef.h>
#include <stdint.h>

/* Recursive helper: counts refactorable divisors of a number
 * given its prime factorisation.
 *
 * p_idx   : current prime index being processed
 * d       : divisor currently constructed
 * tau     : divisor count of the current divisor
 * primes  : array of prime factors
 * exps    : array of corresponding exponents
 * pcnt    : total number of distinct primes
 */
static seq_t
_refactorable_count_rec(size_t p_idx, idx_t d, idx_t tau,
                        const idx_t *primes, const idx_t *exps,
                        size_t pcnt)
{
    if (p_idx == pcnt) {
        /* d is a divisor of the original number.
           It is refactorable if d % tau == 0. */
        return (d % tau == 0) ? (seq_t)1 : (seq_t)0;
    }

    seq_t total = 0;
    idx_t d_mult = 1;     /* power of current prime */
    idx_t tau_factor = 1; /* (exponent+1) for current prime */

    for (idx_t e = 0; e <= exps[p_idx]; ++e) {
        idx_t next_tau = tau * tau_factor;
        total += _refactorable_count_rec(p_idx + 1, d * d_mult,
                                         next_tau, primes, exps, pcnt);
        d_mult *= primes[p_idx];
        tau_factor++;
    }
    return total;
}

/* Small primes up to 100, sufficient for n ≤ 10000. */
static const idx_t PRIMES[] = {2,3,5,7,11,13,17,19,23,29,
                               31,37,41,43,47,53,59,61,67,71,
                               73,79,83,89,97};
static const size_t NUM_PRIMES = sizeof(PRIMES)/sizeof(PRIMES[0]);

/* Main sequence function. */
seq_t a(idx_t n)
{
    if (n == 0) return 0;  /* undefined by OEIS, but safe */

    idx_t rem = n;
    idx_t primes[NUM_PRIMES];
    idx_t exps  [NUM_PRIMES];
    size_t pcnt = 0;

    /* Factorise n.  Only primes up to sqrt(n) are needed. */
    for (size_t i = 0; i < NUM_PRIMES && PRIMES[i] * PRIMES[i] <= rem; ++i) {
        if (rem % PRIMES[i] == 0) {
            idx_t e = 0;
            while (rem % PRIMES[i] == 0) {
                rem /= PRIMES[i];
                e++;
            }
            primes[pcnt] = PRIMES[i];
            exps[pcnt]   = e;
            pcnt++;
        }
    }
    /* If a prime factor larger than sqrt(n) remains. */
    if (rem > 1) {
        primes[pcnt] = rem;
        exps[pcnt]   = 1;
        pcnt++;
    }

    return _refactorable_count_rec(0, (idx_t)1, (idx_t)1,
                                   primes, exps, pcnt);
}
