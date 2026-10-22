/*
 * A349629  Numerators of the Dirichlet inverse of the abundancy index,
 *          sigma(n)/n.
 * https://oeis.org/A349629
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  sign, frac
 * Formula:   not stated
 * Range:     i8 n <= 15, i16 n <= 5053, i32 n <= 8988, i64 n <= 8988,
 *            i128 n <= 8988, big any n
 * Helpers:   gcd
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Dirichlet inverse of the abundancy index.
 * For a prime power p^k, the inverse g(p^k) satisfies
 *     g(1) = 1,
 *     g(p^k) = -∑_{i=1..k} f(p^i) * g(p^{k-i}),
 * where f(p^i) = σ(p^i)/p^i in lowest terms.
 * The function returns the numerator of g(n) in lowest terms.   */
#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>
#include "../utils/gcd.h"

static seq_t abs_seq(seq_t x) { return x < 0 ? -x : x; }

/* Compute the numerator and denominator of g(p^k).  */
static void g_prime_power(uint64_t p, uint64_t k,
                          seq_t *out_num, seq_t *out_den)
{
    size_t sz = (size_t)k + 1;
    seq_t *fnum = (seq_t *)malloc(sz * sizeof(seq_t));
    seq_t *fden = (seq_t *)malloc(sz * sizeof(seq_t));
    seq_t *gnum = (seq_t *)malloc(sz * sizeof(seq_t));
    seq_t *gden = (seq_t *)malloc(sz * sizeof(seq_t));
    if (!fnum || !fden || !gnum || !gden) {
        /* fallback: return 0 on allocation failure */
        *out_num = 0;
        *out_den = 1;
        return;
    }

    /* base case k = 0 */
    fnum[0] = 1;
    fden[0] = 1;
    gnum[0] = 1;
    gden[0] = 1;

    seq_t pow = 1;      /* p^i             */
    seq_t sigma = 1;    /* σ(p^i)          */
    for (uint64_t i = 1; i <= k; ++i) {
        pow = pow * (seq_t)p;
        sigma += pow;                       /* σ(p^i) = σ(p^{i-1}) + p^i   */
        fnum[i] = sigma;
        fden[i] = pow;
        seq_t g = gcd(abs_seq(fnum[i]), fden[i]);
        fnum[i] /= g;
        fden[i] /= g;
    }

    for (uint64_t kk = 1; kk <= k; ++kk) {
        /* sum = Σ_{i=1..kk} f(p^i) * g(p^{kk-i}) */
        seq_t sum_num = 0;
        seq_t sum_den = 1;

        for (uint64_t i = 1; i <= kk; ++i) {
            seq_t a_num = fnum[i];
            seq_t a_den = fden[i];
            seq_t b_num = gnum[kk - i];
            seq_t b_den = gden[kk - i];

            seq_t prod_num = a_num * b_num;
            seq_t prod_den = a_den * b_den;
            seq_t gtmp = gcd(abs_seq(prod_num), prod_den);
            prod_num /= gtmp;
            prod_den /= gtmp;

            seq_t new_num = sum_num * prod_den + prod_num * sum_den;
            seq_t new_den = sum_den * prod_den;
            gtmp = gcd(abs_seq(new_num), new_den);
            new_num /= gtmp;
            new_den /= gtmp;

            sum_num = new_num;
            sum_den = new_den;
        }

        gnum[kk] = -sum_num;
        gden[kk] = sum_den;
        seq_t gfinal = gcd(abs_seq(gnum[kk]), gden[kk]);
        gnum[kk] /= gfinal;
        gden[kk] /= gfinal;
    }

    *out_num = gnum[k];
    *out_den = gden[k];

    free(fnum);
    free(fden);
    free(gnum);
    free(gden);
}

seq_t a(idx_t n)
{
    if (n == 1)
        return 1;            /* g(1) = 1 */

    uint64_t N = (uint64_t)n;
    uint64_t primes[16];
    uint64_t exps[16];
    int pc = 0;

    /* factor 2 separately */
    if ((N & 1ULL) == 0) {
        uint64_t e = 0;
        while ((N & 1ULL) == 0) {
            ++e;
            N >>= 1;
        }
        primes[pc] = 2;
        exps[pc] = e;
        ++pc;
    }

    for (uint64_t p = 3; p * p <= N; p += 2) {
        if (N % p == 0) {
            uint64_t e = 0;
            while (N % p == 0) {
                ++e;
                N /= p;
            }
            primes[pc] = p;
            exps[pc] = e;
            ++pc;
        }
    }
    if (N > 1) {                /* remaining prime */
        primes[pc] = N;
        exps[pc] = 1;
        ++pc;
    }

    /* compute product of g(p^e) over all prime powers */
    seq_t num = 1;
    seq_t den = 1;

    for (int i = 0; i < pc; ++i) {
        seq_t cnum, cden;
        g_prime_power(primes[i], exps[i], &cnum, &cden);

        seq_t new_num = num * cnum;
        seq_t new_den = den * cden;
        seq_t gtmp = gcd(abs_seq(new_num), new_den);
        new_num /= gtmp;
        new_den /= gtmp;

        if (new_num == 0)        /* numerator zero => result zero */
            return 0;

        num = new_num;
        den = new_den;
    }

    return num;                  /* return numerator only */
}
