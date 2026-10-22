/*
 * A334490  a(n) = Sum_{d|n} gcd(d, sigma(d)).
 * https://oeis.org/A334490
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Sum_{d|n} gcd(d, sigma(d))
 * Range:     u8 n <= 359, u16 n <= 9099, u32 n <= 9099, u64 n <= 9099,
 *            u128 n <= 9099, big any n
 * Helpers:   gcd
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computes sigma(d) multiplicatively from the prime factorisation of d */
/* Uses depth‑first generation of all divisors of n */

#include <stdint.h>
#include "../utils/gcd.h"

/* ----------------------------------------------------------------------
   Helper: factorise an unsigned 64‑bit integer n.
   Result: arrays primes[0..cnt-1] with the distinct prime divisors
           and exps[0..cnt-1] with their exponents.
   ---------------------------------------------------------------------- */
static void
factorise_uint64(uint64_t n, uint64_t *primes, int *exps, int *cnt)
{
    *cnt = 0;
    for (uint64_t p = 2; p * p <= n; ++p) {
        if ((n % p) == 0) {
            int e = 0;
            while ((n % p) == 0) {
                n /= p;
                ++e;
            }
            primes[(*cnt)] = p;
            exps[(*cnt)]   = e;
            ++(*cnt);
        }
    }
    if (n > 1) {            /* remaining prime factor */
        primes[(*cnt)] = n;
        exps[(*cnt)]   = 1;
        ++(*cnt);
    }
}

/* ----------------------------------------------------------------------
   Depth‑first generation of divisors of n.
   idx      : current position in the prime lists.
   cur      : product of the chosen powers of the previous primes.
   sigmaEt  : multiplicative product of the sigma(p^e) for the primes
              processed so far.
   primes   : array of prime factors of n.
   exps     : array of exponents of the prime factors of n.
   cnt      : number of distinct prime factors.
   sum_out  : accumulator for the final answer.
   ---------------------------------------------------------------------- */
static void
dfs_divisors(int idx, uint64_t cur, seq_t sigmaEt,
             uint64_t *primes, int *exps, int cnt, seq_t *sum_out)
{
    if (idx == cnt) {
        *sum_out += gcd((seq_t)cur, sigmaEt);
        return;
    }

    uint64_t p = primes[idx];
    uint64_t cur2 = cur;
    seq_t sigmaPrime = 1;   /* sigma(p^0) */
    uint64_t p_pow = 1;     /* p^0 */

    for (int e = 0; e <= exps[idx]; ++e) {
        seq_t newSigmaEt = sigmaEt * sigmaPrime;
        dfs_divisors(idx + 1, cur2, newSigmaEt,
                     primes, exps, cnt, sum_out);

        if (e < exps[idx]) {              /* prepare next exponent */
            p_pow *= p;                  /* p^{e+1} */
            sigmaPrime += p_pow;         /* sum_{i=0}^{e+1} p^i */
            cur2 *= p;
        }
    }
}

/* ----------------------------------------------------------------------
   Main sequence function
   ---------------------------------------------------------------------- */
seq_t a(idx_t n)
{
    if (n == 0)                    /* safety for index 0 */
        return 0;

    /* factorise n once -------------------------------------------- */
    uint64_t primes[64];
    int      exps[64];
    int      cnt;
    factorise_uint64((uint64_t)n, primes, exps, &cnt);

    /* generate all divisors of n and accumulate the sum ---------- */
    seq_t sum = 0;
    dfs_divisors(0, 1ULL, 1, primes, exps, cnt, &sum);

    return sum;
}
