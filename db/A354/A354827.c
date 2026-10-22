/*
 * A354827  Numerators of Dirichlet inverse of fraction A003961(n) / sigma(n).
 * https://oeis.org/A354827
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  sign, frac
 * Formula:   a(n) is the numerator of the reduced fraction * g(n) where g is
 *            the Dirichlet inverse of * f(n) = A003961(n)/σ(n). * The
 *            definition of A003961 is: * A003961(n) = ∏_{p^e‖n}
 *            next_prime(p)^e, * where next_prime(p) is the least prime > p. *
 *            Thus f(p^e) = next_prime(p)^e / σ(p^e), * where σ(p^e) = (p^...
 * Range:     i8 n <= 8, i16 n <= 26, i32 n <= 255, i64 n <= 2047,
 *            i128 n <= 9694, big any n
 * Helpers:   build_primes, build_spf, gcd
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>
#include "../utils/gcd.h"
#include "../utils/build_primes.h"   /* build_primes, build_spf are included indirectly */
#include "../utils/build_spf.h"

static idx_t *primes = NULL;        /* list of primes up to LIMIT */
static size_t primes_cnt = 0;
static idx_t *spf = NULL;           /* smallest prime factor array */
static idx_t spf_size = 0;
#define LIMIT 20000                  /* enough for p up to 10000 */

/* initialise prime list and spf array once */
static void init_tables(void)
{
    if (primes == NULL) {
        build_primes(LIMIT, &primes, &primes_cnt);
        spf_size = LIMIT + 1;
        spf = (idx_t *)malloc(sizeof(idx_t) * spf_size);
        build_spf(LIMIT, &spf);
    }
}

/* return the next prime after p (p is guaranteed to be prime) */
static idx_t next_prime(idx_t p)
{
    for (size_t i = 0; i < primes_cnt; ++i)
        if (primes[i] > p)
            return primes[i];
    /* should not happen for n <= 10000 */
    return primes[primes_cnt - 1];
}

/* compute f(p^e)  (f = next_prime^e / sigma(p^e)) as a reduced fraction
 *   out_num, out_den receive the numerator and denominator
 */
static void compute_f_prime(idx_t p, size_t e, seq_t *out_num, seq_t *out_den)
{
    seq_t nextp = (seq_t)next_prime(p);

    /* nextp^e */
    seq_t npow = 1;
    for (size_t i = 1; i <= e; ++i)
        npow *= nextp;

    /* sigma(p^e) = (p^(e+1)-1)/(p-1) */
    seq_t p_pow_next = 1;
    for (size_t i = 1; i <= e + 1; ++i)
        p_pow_next *= (seq_t)p;
    seq_t sigma = (p_pow_next - 1) / ((seq_t)p - 1);

    /* reduce */
    seq_t g = gcd(npow, sigma);
    *out_num = npow / g;
    *out_den = sigma / g;
}

/* compute g(p^k) for a given prime power using the recurrence.
 *  The result for g(p^k) is returned in out_num, out_den (already reduced).
 */
static void compute_g_prime(idx_t p, size_t k, seq_t *out_num, seq_t *out_den)
{
    /* pre‑compute f(i) for i = 1..k as fractions */
    seq_t f_num[14], f_den[14];   /* k <= 13 for n <= 10000 */
    f_num[0] = 1; f_den[0] = 1;   /* f(p^0)=1 */

    for (size_t i = 1; i <= k; ++i)
        compute_f_prime(p, i, &f_num[i], &f_den[i]);

    /* g(0) = 1/1 */
    seq_t g_num[14], g_den[14];
    g_num[0] = 1;
    g_den[0] = 1;

    for (size_t kk = 1; kk <= k; ++kk) {
        seq_t sum_num = 0;
        seq_t sum_den = 1;            /* sum = 0/1 initially */

        for (size_t i = 1; i <= kk; ++i) {
            seq_t term_num = f_num[i] * g_num[kk - i];
            seq_t term_den = f_den[i] * g_den[kk - i];

            /* sum = sum + term */
            seq_t new_num = sum_num * term_den + term_num * sum_den;
            seq_t new_den = sum_den * term_den;

            seq_t gtmp = gcd(new_num, new_den);
            sum_num = new_num / gtmp;
            sum_den = new_den / gtmp;
            if (sum_den < 0) {
                sum_num = -sum_num;
                sum_den = -sum_den;
            }
        }

        /* g(kk) = - sum */
        seq_t gn = -sum_num;
        seq_t gd = sum_den;

        seq_t gtmp = gcd(gn, gd);
        gn /= gtmp;
        gd /= gtmp;
        if (gd < 0) {
            gn = -gn;
            gd = -gd;
        }

        g_num[kk] = gn;
        g_den[kk] = gd;
    }

    *out_num = g_num[k];
    *out_den = g_den[k];
}

/* factor n into arrays of primes and exponents (maximum 10 distinct primes for n<=10000) */
static void factorize(idx_t n, idx_t *primes_out, size_t *exps_out, size_t *cnt_out)
{
    size_t cnt = 0;
    while (n > 1) {
        idx_t p = spf[n];
        size_t e = 0;
        while (n % p == 0) {
            n /= p;
            ++e;
        }
        primes_out[cnt] = p;
        exps_out[cnt] = e;
        ++cnt;
    }
    *cnt_out = cnt;
}

/* main sequence function */
seq_t a(idx_t n)
{
    init_tables();                 /* ensure primes/spf ready */

    if (n == 1)
        return (seq_t)1;

    /* factor n */
    idx_t fac_primes[15];
    size_t fac_exps[15];
    size_t fac_cnt;
    factorize(n, fac_primes, fac_exps, &fac_cnt);

    /* product of g(p^e) */
    seq_t num = 1;
    seq_t den = 1;

    for (size_t i = 0; i < fac_cnt; ++i) {
        seq_t gnum, gden;
        compute_g_prime(fac_primes[i], fac_exps[i], &gnum, &gden);

        /* multiply with accumulation, reduce each step */
        num *= gnum;
        den *= gden;

        seq_t g_tmp = gcd(num, den);
        num /= g_tmp;
        den /= g_tmp;
        if (den < 0) {
            num = -num;
            den = -den;
        }
    }
    /* Return numerator of reduced fraction g(n) */
    return num;
}
