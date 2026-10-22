/*
 * A351395  Sum of the divisors of n that are either squarefree, prime powers,
 *          or both.
 * https://oeis.org/A351395
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Σ_{d|n, d is squarefree or a prime power} d
 * Range:     u8 n <= 137, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Equivalent closed‑form: a(n) = Π_{p^e∥n} (1+p) + Σ_{p^e∥n} (p + p² + … + p^e) – Σ_{p^e∥n} p */

/* Computes the sum of all divisors of n that are squarefree or a prime power. */
#include <stdint.h>
#include <stdbool.h>

seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0;          /* undefined for 0, just guard */
    idx_t nn = n;                          /* working copy of n */
    seq_t sf_sum = (seq_t)1;               /* product of (1 + p) for each prime p */
    seq_t pp_sum = (seq_t)0;               /* sum of prime‑power divisors, excluding 1 */
    seq_t p_sum  = (seq_t)0;               /* sum of distinct primes dividing n */

    for (idx_t p = 2; (seq_t)p * (seq_t)p <= nn; )
    {
        if (nn % p == 0)                    /* p is a prime factor */
        {
            idx_t e = 0;
            while (nn % p == 0) {            /* extract exponent */
                nn /= p;
                ++e;
            }

            /* update squarefree part */
            sf_sum *= ( (seq_t)p + (seq_t)1 );
            /* sum of distinct primes */
            p_sum += (seq_t)p;

            /* geometric series for p^1 … p^e */
            seq_t term = (seq_t)p;
            for (idx_t k = 1; k <= e; ++k) {
                pp_sum += term;
                term *= (seq_t)p;
            }
        }

        /* step to next potential divisor: 2 → 3 → 5 → 7 … */
        if (p == 2) p = 3;
        else        p += 2;
    }

    /* remaining prime factor, if any */
    if (nn > 1) {
        seq_t p_seq = (seq_t)nn;
        sf_sum *= (p_seq + (seq_t)1);
        p_sum  += p_seq;
        pp_sum += p_seq;                     /* only the first power remains */
    }

    /* union of squarefree and prime‑power divisors */
    return sf_sum + pp_sum - p_sum;
}
