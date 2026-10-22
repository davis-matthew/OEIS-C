/*
 * A307076  Expansion of 1/(1 - Sum_{k>=1} lambda(k)*x^k), where lambda() is
 *          the Liouville function (A008836).
 * https://oeis.org/A307076
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign
 * Formula:   a(0) = 1 * a(n) = Σ_{k=1..n} λ(k) * a(n-k) (n ≥ 1) * where λ(k)
 *            = (-1)^{Ω(k)} is the Liouville function. * * The implementation
 *            keeps a memoised array for a(n) and * precomputes λ(k) by a
 *            simple smallest‑prime‑factor sieve. * It works for any build
 *            where `seq_t` is an arithmetic type or * an arbi...
 * Range:     i8 n <= 19, i16 n <= 35, i32 n <= 47, i64 n <= 47, i128 n <= 47,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdint.h>
#include <stdlib.h>

/* static storage for the computed sequence */
static seq_t *a_vals = NULL;
static size_t a_cap = 0;          /* number of a_values stored */

/* Liouville function values λ(k) */
static int8_t *lambda = NULL;     /* λ[0] unused, λ[1] = 1  */
static size_t lambda_cap = 0;     /* largest index present */

/* Smallest prime factor table for computing Ω(k) */
static uint32_t *spf = NULL;      /* spf[0] unused, spf[1] = 1  */
static size_t spf_cap = 0;        /* largest index present */

/* Forward declaration */
static void ensure_spf_and_lambda_up_to(idx_t n);

/* Return λ(k) for k ≥ 1 (definition λ(1)=1) */
static int8_t liouville(idx_t k)
{
    return lambda[k];
}

/* Build/extend spf[] and λ[] up to n (inclusive) */
static void ensure_spf_and_lambda_up_to(idx_t n)
{
    if (n < lambda_cap) return;          /* already computed */

    /* Resize λ array */
    size_t new_cap = (size_t)n + 1;
    int8_t *tmp_lambda = (int8_t *)realloc(lambda, new_cap * sizeof(int8_t));
    if (!tmp_lambda) exit(EXIT_FAILURE);
    lambda = tmp_lambda;

    /* Resize spf array */
    if (n + 1 > spf_cap) {
        size_t spf_new_cap = (size_t)n + 1;
        uint32_t *tmp_spf = (uint32_t *)realloc(spf, spf_new_cap * sizeof(uint32_t));
        if (!tmp_spf) exit(EXIT_FAILURE);
        spf = tmp_spf;
        spf_cap = spf_new_cap;
    }

    /* Initialise spf array for the new range (simple sieve) */
    for (idx_t i = 1; i <= n; ++i) spf[i] = 0;
    spf[1] = 1;
    for (idx_t i = 2; i * i <= n; ++i) {
        if (spf[i] == 0) {
            spf[i] = (uint32_t)i;
            for (idx_t j = i * i; j <= n; j += i)
                if (spf[j] == 0) spf[j] = (uint32_t)i;
        }
    }
    for (idx_t i = 2; i <= n; ++i)
        if (spf[i] == 0) spf[i] = (uint32_t)i;

    /* Compute λ for indices beyond the previous cap */
    for (idx_t m = lambda_cap + 1; m <= n; ++m) {
        idx_t t = m;
        unsigned omega = 0;
        while (t > 1) {
            ++omega;
            t /= spf[t];
        }
        lambda[m] = (omega & 1) ? -1 : 1;
    }

    lambda_cap = (size_t)n;
}

/* Public function computing a(n) */
seq_t a(idx_t n)
{
    /* Initialise a(0) once */
    if (a_cap == 0) {
        size_t cap = 1;
        seq_t *tmp = (seq_t *)realloc(a_vals, cap * sizeof(seq_t));
        if (!tmp) exit(EXIT_FAILURE);
        a_vals = tmp;
        a_vals[0] = 1;
        a_cap = cap;
    }

    /* If requested value already computed, return it */
    if (n < a_cap) return a_vals[n];

    /* Ensure λ(k) up to n is available */
    ensure_spf_and_lambda_up_to(n);

    /* Expand a_vals array if necessary */
    if (n + 1 > a_cap) {
        size_t new_cap = (size_t)n + 1;
        seq_t *tmp = (seq_t *)realloc(a_vals, new_cap * sizeof(seq_t));
        if (!tmp) exit(EXIT_FAILURE);
        a_vals = tmp;

        /* Compute a(i) for all i that are now requested */
        for (idx_t i = a_cap; i <= n; ++i) {
            seq_t sum = (seq_t)0;
            for (idx_t k = 1; k <= i; ++k) {
                sum += (seq_t)lambda[k] * a_vals[i - k];
            }
            a_vals[i] = sum;
        }
        a_cap = new_cap;
    }

    return a_vals[n];
}
