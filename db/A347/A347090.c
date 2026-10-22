/*
 * A347090  Dirichlet inverse of A332844.
 * https://oeis.org/A347090
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  sign, mult, look
 * Formula:   not stated
 * Range:     i8 n <= 48, i16 n <= 9885, i32 n <= 9885, i64 n <= 9885,
 *            i128 n <= 9885, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* 
 * Formula: a(n) = Dirichlet inverse of A332844
 * a(1) = 1;
 * For n>1: a(n) = - Σ_{d|n, d<n} f(n/d) * a(d),
 * where f is multiplicative:
 *   f(1) = 1,
 *   f(p) = p+1,
 *   and for e >= 2, f(p^e) = σ(p^e) + f(p^{e-2}),
 *      σ(p^e) = (p^{e+1}-1)/(p-1).
 * The sequence a(n) is thus multiplicative and can be computed
 * by prime‑factorising n and multiplying the prime‑power values
 * a(p^e).
 */

#include <stdlib.h>
#include <stdint.h>

/* Types supplied by the build */
typedef idx_t idx_t;   /* unsigned integer, e.g., uint64_t */
typedef seq_t seq_t;   /* signed integer, may be big integer */

/* Local helpers ----------------------------------------------------- */

/* Compute f(p^e) for a prime p and exponent e.  f is multiplicative
 * and satisfies f(1)=1, f(p)=p+1, and for e>=2: f(p^e)=σ(p^e)+f(p^{e-2}). */
static seq_t f_prime_power(idx_t p, idx_t e)
{
    if (e == (idx_t)0) return (seq_t)1;
    /* The exponent e is small for n <= 1e4 (max ~13 for p=2). */
    seq_t f[65];
    f[0] = (seq_t)1;
    f[1] = (seq_t)p + (seq_t)1;

    for (idx_t k = (idx_t)2; k <= e; ++k) {
        /* Compute σ(p^k) = (p^{k+1}-1)/(p-1) */
        seq_t pow = (seq_t)1;
        for (idx_t i = (idx_t)0; i < k + (idx_t)1; ++i)
            pow *= (seq_t)p;          /* p^(k+1) */
        seq_t sigma = (pow - (seq_t)1) / ((seq_t)p - (seq_t)1);
        f[k] = sigma + f[k - (idx_t)2];
    }
    return f[e];
}

/* Compute a(p^e), the Dirichlet inverse of f at the prime power p^e. */
static seq_t a_prime_power(idx_t p, idx_t e)
{
    if (e == (idx_t)0) return (seq_t)1;

    /* f values up to e */
    seq_t f[65];
    f[0] = (seq_t)1;
    f[1] = (seq_t)p + (seq_t)1;
    for (idx_t k = (idx_t)2; k <= e; ++k) {
        seq_t pow = (seq_t)1;
        for (idx_t i = (idx_t)0; i < k + (idx_t)1; ++i)
            pow *= (seq_t)p;
        seq_t sigma = (pow - (seq_t)1) / ((seq_t)p - (seq_t)1);
        f[k] = sigma + f[k - (idx_t)2];
    }

    /* a values up to e, using convolution a * f = ε */
    seq_t a[65];
    a[0] = (seq_t)1;
    for (idx_t k = (idx_t)1; k <= e; ++k) {
        seq_t sum = (seq_t)0;
        for (idx_t j = (idx_t)0; j < k; ++j) {
            sum += a[j] * f[k - j];
        }
        a[k] = -sum;
    }
    return a[e];
}

/* Main sequence function --------------------------------------------- */

seq_t a(idx_t n)
{
    /* Static cache for smallest prime factor (spf) up to current size. */
    static idx_t *spf = NULL;
    static idx_t spf_size = (idx_t)0;

    /* Grow spf table if required. */
    if (n >= spf_size) {
        idx_t new_size = n + (idx_t)1;
        idx_t *tmp = (idx_t *)realloc(spf, new_size * sizeof(idx_t));
        if (!tmp) return (seq_t)0;             /* allocation failed */
        spf = tmp;
        for (idx_t i = (idx_t)0; i < new_size; ++i) spf[i] = (idx_t)0;
        for (idx_t i = (idx_t)2; i < new_size; ++i) {
            if (spf[i] == (idx_t)0) {
                spf[i] = i;
                if (i * i < new_size) {
                    for (idx_t j = i * i; j < new_size; j += i)
                        if (spf[j] == (idx_t)0) spf[j] = i;
                }
            }
        }
        if (new_size > (idx_t)1) spf[1] = (idx_t)1;
        spf_size = new_size;
    }

    if (n == (idx_t)1) return (seq_t)1;

    seq_t result = (seq_t)1;
    idx_t tmp = n;
    while (tmp > (idx_t)1) {
        idx_t p = spf[tmp];
        idx_t exp = (idx_t)0;
        while (tmp % p == (idx_t)0) {
            tmp /= p;
            ++exp;
        }
        result *= a_prime_power(p, exp);
    }
    return result;
}
