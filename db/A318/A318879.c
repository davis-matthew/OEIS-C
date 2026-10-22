/*
 * A318879  a(n) = Sum_{d|n} [d-(2*phi(d)) > 0]*(d-(2*phi(d))).
 * https://oeis.org/A318879
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Σ_{d|n} [d > 2·φ(d)] · (d – 2·φ(d))
 * Range:     u8 n <= 239, u16 n <= 9724, u32 n <= 9724, u64 n <= 9724,
 *            u128 n <= 9724, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

/* Recursive enumeration of all divisors of n.
   cur_d  – current divisor being constructed
   cur_phi – φ(cur_d) of that divisor
   primes, exps – prime factorisation of n
   np – number of distinct primes
   sum – accumulator for the result                       */
static void
_enum_divs(int idx,
           seq_t cur_d,
           seq_t cur_phi,
           const uint64_t *primes,
           const int *exps,
           int np,
           seq_t *sum)
{
    if (idx == np) {
        /* Add only when the difference is strictly positive.          */
        if (cur_d > (cur_phi << 1))
            *sum += cur_d - (cur_phi << 1);
        return;
    }

    /* Skip the current prime (exponent 0).                          */
    _enum_divs(idx + 1, cur_d, cur_phi, primes, exps, np, sum);

    /* Consider exponent k = 1 … exps[idx].                         */
    uint64_t p     = primes[idx];
    seq_t    power = 1;   /* p^k                                     */
    seq_t    phi_pk = 1;  /* φ(p^k)                                   */
    for (int k = 1; k <= exps[idx]; ++k) {
        power *= (seq_t)p;                      /* p^k   */
        if (k == 1)
            phi_pk = (seq_t)(p - 1);            /* φ(p)   */
        else
            phi_pk *= (seq_t)p;                 /* φ(p^k) */

        _enum_divs(idx + 1,
                   cur_d * power,
                   cur_phi * phi_pk,
                   primes,
                   exps,
                   np,
                   sum);
    }
}

seq_t a(idx_t n)
{
    if (n == 0)
        return 0; /* undefined for n=0, but returns 0 safely */

    uint64_t N = (uint64_t)n;

    /* Factorise N.                                                  */
    uint64_t primes[64];
    int exps[64];
    int nprimes = 0;
    uint64_t tmp = N;

    for (uint64_t p = 2; p * p <= tmp; ++p) {
        if (tmp % p == 0) {
            int e = 0;
            while (tmp % p == 0) {
                tmp /= p;
                ++e;
            }
            primes[nprimes]  = p;
            exps[nprimes]    = e;
            ++nprimes;
        }
    }
    if (tmp > 1) {         /* remaining prime factor               */
        primes[nprimes]  = tmp;
        exps[nprimes]    = 1;
        ++nprimes;
    }

    seq_t sum = 0;
    _enum_divs(0, (seq_t)1, (seq_t)1, primes, exps, nprimes, &sum);
    return sum;
}
