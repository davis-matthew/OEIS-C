/*
 * A323779  a(n) = numerator of Sum_{d|n} (tau(d)/sigma(d)) where tau(k) = the
 *          number of the divisors of k (A000005) and sigma(k) = the sum of
 *          the divisors of k (A000203).
 * https://oeis.org/A323779
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, frac
 * Formula:   a(n) = numerator of sum_{d|n} tau(d)/sigma(d) where tau(d) =
 *            number of divisors of d, and sigma(d) = sum of divisors of d
 * Range:     u8 n <= 7, u16 n <= 31, u32 n <= 63, u64 n <= 63, u128 n <= 63,
 *            big any n
 * Helpers:   build_spf, gcd, sigma_term
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include "../utils/gcd.h"
#include "../utils/build_spf.h"
#include "../utils/sigma_term.h"

/* Pre‑computed smallest prime factor table up to spf_limit. */
static idx_t *spf = NULL;
static idx_t spf_limit = 0;

/* Ensure spf[0..n] are filled. */
static void ensure_spf(idx_t n)
{
    if (n <= spf_limit)
        return;
    if (spf != NULL)
        free(spf);
    build_spf(n, &spf);
    spf_limit = n;
}

/* Recursive traversal over the prime factorization of n.
   cur_tau  = product of (exp_j + 1) for primes already chosen
   cur_sigma= product of sigma_term for primes already chosen
   num,den  = current reduced sum */
static void walk_divs(const uint64_t *primes, const int *exps, int nf,
                      seq_t cur_tau, seq_t cur_sigma,
                      seq_t *num, seq_t *den)
{
    if (nf == 0) {
        /* Add cur_tau / cur_sigma to the running sum. */
        seq_t new_num = (*num) * cur_sigma + (*den) * cur_tau;
        seq_t new_den = (*den) * cur_sigma;
        seq_t g = gcd(new_num, new_den);
        *num = new_num / g;
        *den = new_den / g;
        return;
    }

    uint64_t p = primes[0];
    int max_exp = exps[0];
    /* For each exponent e = 0..max_exp, recurse. */
    seq_t tau = cur_tau;
    seq_t sigma = cur_sigma;
    /* e = 0 branch */
    walk_divs(primes + 1, exps + 1, nf - 1, tau, sigma, num, den);

    /* e >= 1 branches */
    for (int e = 1; e <= max_exp; ++e) {
        /* Update tau and sigma for this exponent e. */
        seq_t term_sigma = sigma_term((seq_t)p, e);
        tau = cur_tau * (e + 1);
        sigma = cur_sigma * term_sigma;
        walk_divs(primes + 1, exps + 1, nf - 1, tau, sigma, num, den);
    }
}

/* Main sequence function. */
seq_t a(idx_t n)
{
    if (n == 0)
        return 0;          /* undefined; not used */

    /* 1. Prepare spf table. */
    ensure_spf(n);

    /* 2. Factorize n. */
    uint64_t primes[64];
    int exps[64];
    int nf = 0;
    idx_t tmp = n;
    while (tmp > 1) {
        uint64_t p = spf[tmp];
        int e = 0;
        while (tmp % p == 0) {
            tmp /= p;
            ++e;
        }
        primes[nf] = p;
        exps[nf] = e;
        ++nf;
    }

    /* 3. Traverse divisors and accumulate the sum. */
    seq_t num = 0;     /* numerator of the sum so far */
    seq_t den = 1;     /* denominator of the sum so far */
    walk_divs(primes, exps, nf, (seq_t)1, (seq_t)1, &num, &den);

    /* 4. Return the numerator of the reduced fraction. */
    return num;
}
