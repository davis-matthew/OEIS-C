/*
 * A347042  Number of divisors d > 1 of n such that bigomega(d) divides
 *          bigomega(n), where bigomega = A001222.
 * https://oeis.org/A347042
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = number of divisors d > 1 of n such that bigomega(d)
 *            divides bigomega(n) (bigomega = A001222) Complexity: O( τ(n) *
 *            Ω(n) ) per call, where τ is the divisor count and Ω the total
 *            number of prime factors
 * Range:     u8 n <= 10000, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   bigomega
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include "../utils/bigomega.h"

/* Forward declaration of the recursive helper */
static void dfs_divisors(int idx, bool started, seq_t cur_omega,
                         const seq_t big_omega_n,
                         int pf,
                         uint64_t *primes, unsigned *exps,
                         seq_t *count_ptr);

/* Main function required by the build system */
seq_t a(idx_t n)
{
    if (n == 1)        /* n = 1 has no d > 1 */
        return 0;

    /* Factor n: small enough that trial division is fast for n <= 10000 */
    uint64_t nn = (uint64_t)n;
    /* Statically allocated arrays – 64 factors are more than enough for any
       64‑bit integer (actually at most 20 for n < 10^16) */
    uint64_t primes[64];
    unsigned  exps[64];
    int pf = 0;            /* number of distinct prime factors */

    /* Simple trial division, avoiding overflow on p*p */
    uint64_t tmp = nn;
    for (uint64_t p = 2; p <= tmp / p; ++p) {
        if (tmp % p == 0) {
            unsigned cnt = 0;
            while (tmp % p == 0) {
                tmp /= p;
                ++cnt;
            }
            primes[pf] = p;
            exps[pf]  = cnt;
            ++pf;
        }
    }
    if (tmp > 1) {             /* remaining prime factor */
        primes[pf] = tmp;
        exps[pf]   = 1;
        ++pf;
    }

    /* Compute bigomega(n) once – this function returns a seq_t */
    seq_t big_omega_n = bigomega(n);

    /* Recursive enumeration of all divisors > 1 */
    seq_t count = 0;
    dfs_divisors(0, false, 0, big_omega_n, pf, primes, exps, &count);

    return count;
}

/* Recursively generate all divisors from the factorization.  The arguments
   are:
   idx          – current factor index
   started      – true if we have chosen at least one prime factor
   cur_omega    – sum of exponents of the divisor constructed so far
   big_omega_n  – bigomega(n)
   pf           – number of distinct prime factors
   primes,exps  – factorization data
   count_ptr    – pointer to the running total of satisfying divisors
*/
static void dfs_divisors(int idx, bool started, seq_t cur_omega,
                         const seq_t big_omega_n,
                         int pf,
                         uint64_t *primes, unsigned *exps,
                         seq_t *count_ptr)
{
    if (idx == pf) {
        if (started && (big_omega_n % cur_omega == 0))
            (*count_ptr)++;
        return;
    }

    /* For the current prime, try every exponent from 0 up to its multiplicity */
    for (unsigned e = 0; e <= exps[idx]; ++e) {
        bool next_started = started || (e > 0);
        seq_t next_omega  = cur_omega + (seq_t)e;
        dfs_divisors(idx + 1, next_started, next_omega,
                     big_omega_n, pf, primes, exps, count_ptr);
    }
}
