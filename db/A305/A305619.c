/*
 * A305619  Expansion of e.g.f. log(1 + Sum_{k>=1} x^prime(k)/prime(k)).
 * https://oeis.org/A305619
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  sign
 * Formula:   a(n) = coefficient of x^n/n! in log(1 + Sum_{p prime} x^p/p)
 * Range:     i8 n <= 6, i16 n <= 8, i32 n <= 13, i64 n <= 21, i128 n <= 23,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <stdint.h>

/* seq_t and idx_t are supplied by the build */
#ifdef SEQ_T_IS_BIGINT
/* In the big‑integer build seq_t is an arbitrary‑precision integer type.
   The following code works with any type that supports the usual
   arithmetic operators. */
#else
/* For built‑in integer types the same operators are sufficient. */
#endif

seq_t a(idx_t n)
{
    /* static state: cached data up to the largest n seen so far */
    static idx_t max_n = 0;
    static seq_t *a_vals = NULL;       /* a(n) values */
    static seq_t *fac     = NULL;       /* factorials fac[i] = i!   */
    static bool  *is_prime = NULL;      /* primality flags  */
    static idx_t *primes   = NULL;      /* list of primes      */
    static size_t primes_cap = 0;       /* capacity of primes array */
    static size_t primes_cnt = 0;       /* number of primes      */

    /* If the requested n is already computed, just return the value. */
    if (n <= max_n && a_vals != NULL) {
        return a_vals[n];
    }

    /* ---- resize arrays to cover [0..n] ---- */
    size_t new_cap = (size_t)n + 1;     /* include index n */

    a_vals = (seq_t *)realloc(a_vals, new_cap * sizeof(seq_t));
    fac    = (seq_t *)realloc(fac   , new_cap * sizeof(seq_t));
    is_prime = (bool  *)realloc(is_prime, new_cap * sizeof(bool));

    if (!a_vals || !fac || !is_prime) {
        /* allocation failure – return 0 as a fallback */
        return 0;
    }

    /* ---- compute factorials up to n ---- */
    if (max_n == 0) {
        fac[0] = 1;
    }
    for (idx_t i = max_n + 1; i <= n; ++i) {
        fac[i] = fac[i - 1] * ((seq_t)i);
    }

    /* ---- sieve prime flags up to n ---- */
    for (idx_t i = 2; i <= n; ++i) { is_prime[i] = true; }
    for (idx_t i = 2; i * i <= n; ++i) {
        if (is_prime[i]) {
            for (idx_t j = i * i; j <= n; j += i) {
                is_prime[j] = false;
            }
        }
    }

    /* ---- build primes list up to n ---- */
    /* Count primes first */
    size_t count = 0;
    for (idx_t i = 2; i <= n; ++i) if (is_prime[i]) ++count;

    if (primes_cap < count) {
        idx_t *tmp = (idx_t *)realloc(primes, count * sizeof(idx_t));
        if (!tmp) return 0;
        primes = tmp;
        primes_cap = count;
    }

    primes_cnt = 0;
    for (idx_t i = 2; i <= n; ++i) {
        if (is_prime[i]) primes[primes_cnt++] = i;
    }

    /* ---- compute sequence values from (max_n+1) to n ---- */
    if (max_n < 1) {
        a_vals[1] = 0;     /* base case */
    }

    for (idx_t i = max_n + 1; i <= n; ++i) {
        if (i == 1) {
            a_vals[1] = 0;
            continue;
        }

        seq_t sum = 0;

        /* loop over primes p <= i-1 */
        for (size_t pi = 0; pi < primes_cnt && primes[pi] <= i - 1; ++pi) {
            idx_t p = primes[pi];
            idx_t k = i - p;          /* k = i - p */
            seq_t tmp = fac[i - 1] / fac[k - 1];  /* product of k..i-1 */
            tmp /= p;                 /* divide by p = i - k */
            seq_t term = a_vals[k] * tmp;
            sum += term;
        }

        if (is_prime[i]) {
            a_vals[i] = fac[i - 1] - sum;  /* a_i = (i-1)! - sum */
        } else {
            a_vals[i] = -sum;              /* a_i = -sum       */
        }
    }

    max_n = n;
    return a_vals[n];
}
