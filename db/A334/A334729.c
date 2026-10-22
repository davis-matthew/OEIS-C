/*
 * A334729  a(n) = Product_{d|n} gcd(tau(d), sigma(d)).
 * https://oeis.org/A334729
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Product_{d|n} gcd(tau(d), sigma(d))
 * Range:     u8 n <= 18, u16 n <= 59, u32 n <= 359, u64 n <= 839,
 *            u128 n <= 4619, big any n
 * Helpers:   build_primes_upto, gcd
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computes a(n) for sequence A334729.
 *
 * tau(d)   - number of positive divisors of d
 * sigma(d) - sum of positive divisors of d
 *
 * The implementation factors n, enumerates all divisors recursively,
 * computes tau and sigma for each divisor from its prime factorisation,
 * and multiplies the gcd(tau, sigma) values. */
#include <stdlib.h>
#include <stdint.h>

#include "../utils/build_primes_upto.h"
#include "../utils/gcd.h"

/* --------- Utility functions --------------------------------------------- */

/* Integer power for seq_t type. */
static seq_t pow_seq(seq_t base, int exp)
{
    seq_t res = 1;
    for (int i = 0; i < exp; ++i)
        res *= base;
    return res;
}

/* Recursively enumerate all divisors of n from its prime factorisation. */
static void rec_divisors(const uint64_t *primes, const int *exps, size_t nf,
                         size_t idx, seq_t cur, seq_t tau_cur, seq_t sigma_cur,
                         seq_t *prod)
{
    if (idx == nf) {
        *prod *= gcd(tau_cur, sigma_cur);
        return;
    }

    uint64_t p = primes[idx];
    int    emax = exps[idx];
    seq_t  cur_mult = 1;          /* p^e  */
    for (int e = 0; e <= emax; ++e) {
        /* Sigma part for p^e : (p^{e+1}-1)/(p-1)  (e==0 gives 1) */
        seq_t sigma_part = 1;
        if (e > 0) {
            sigma_part = (pow_seq(p, e + 1) - 1) / (p - 1);
        }
        /* Recurse for the next prime factor */
        rec_divisors(primes, exps, nf, idx + 1,
                     cur * cur_mult,
                     tau_cur * (e + 1),
                     sigma_cur * sigma_part,
                     prod);
        cur_mult *= p;   /* increase p^e for next loop iteration */
    }
}

/* --------- Main sequence function --------------------------------------- */

seq_t a(idx_t n)
{
    /* Handle the forbidden index 0 (not used in OEIS). */
    if (n == 0)
        return 0;

    /* Factorise n by building a list of primes up to n. */
    uint64_t *prime_list = NULL;
    size_t    prime_cnt  = 0;
    build_primes_upto(n, &prime_list, &prime_cnt);

    /* Extract the distinct prime factors of n and their exponents. */
    uint64_t  factors[64];
    int       exps[64];
    size_t    nf = 0;                 /* number of distinct factors */

    uint64_t tmp = n;
    for (size_t i = 0; i < prime_cnt && prime_list[i] * prime_list[i] <= tmp; ++i) {
        uint64_t p = prime_list[i];
        if (tmp % p == 0) {
            int e = 0;
            while (tmp % p == 0) {
                tmp /= p;
                ++e;
            }
            factors[nf] = p;
            exps[nf]    = e;
            ++nf;
        }
    }
    if (tmp > 1) {     /* remaining prime factor > sqrt(n) */
        factors[nf] = tmp;
        exps[nf]    = 1;
        ++nf;
    }

    /* Enumerate all divisors and compute the product. */
    seq_t result = 1;
    rec_divisors(factors, exps, nf, 0, (seq_t)1, (seq_t)1, (seq_t)1, &result);

    /* Clean up allocated memory. */
    free(prime_list);

    return result;
}
