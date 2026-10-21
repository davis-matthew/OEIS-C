/*
 * A262400  Let f(x) = 1 + Sum_{j>=4} (|mu(j)| - |mu(j-1)|)*x^j, where mu(n)
 *          is the Möbius function (A008683). Then a(n) is n times the
 *          coefficient of x^n in the expansion of log(f(x)).
 * https://oeis.org/A262400
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign
 * Formula:   a(n) = n * (coefficient of x^n in log(f(x)))
 * Range:     i8 n <= 20, i16 n <= 45, i32 n <= 94, i64 n <= 192,
 *            i128 n <= 387, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Here f(x) = 1 + Σ_{j≥4} (|μ(j)|−|μ(j−1)|) x^j   (μ Möbius). */
/* The coefficient of x^n in log(f(x)) is computed by the convolution */
/* recurrence B[n] = n*a[n] – Σ_{k=1}^{n-1} B[k]*a[n−k],  B[0]=0. */
/* a[n] = |μ(n)|−|μ(n−1)| (for n≥4, otherwise 0). */

#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>

/* static cache for memoised values */
static seq_t *a_coeff = NULL;      /* a[n] for n up to current max */
static seq_t *B       = NULL;      /* B[n] = n*coeff of x^n in log(f(x)) */
static idx_t  max_n   = 0;          /* largest n computed so far */

/* linear sieve for the Möbius function up to limit */
static void compute_mu_and_a(idx_t limit)
{
    /* allocate temporary arrays */
    bool *is_comp = (bool*)calloc((size_t)limit + 1, sizeof(bool));
    int8_t *mu    = (int8_t*)calloc((size_t)limit + 1, sizeof(int8_t));
    if (!is_comp || !mu) { exit(1); }

    /* list of primes */
    size_t primes_cap = 100;
    size_t primes_sz = 0;
    int *primes = (int*)malloc(primes_cap * sizeof(int));
    if (!primes) { free(is_comp); free(mu); exit(1); }

    /* Möbius initialization */
    mu[1] = 1;

    for (idx_t i = 2; i <= limit; ++i) {
        if (!is_comp[i]) {            /* i is prime */
            if (primes_sz == primes_cap) {
                primes_cap *= 2;
                int *tmp = (int*)realloc(primes, primes_cap * sizeof(int));
                if (!tmp) { free(is_comp); free(mu); free(primes); exit(1); }
                primes = tmp;
            }
            primes[primes_sz++] = (int)i;
            mu[i] = -1;
        }
        for (size_t pi = 0; pi < primes_sz; ++pi) {
            idx_t p = primes[pi];
            idx_t ip = i * p;
            if (ip > limit) break;
            is_comp[ip] = true;
            if (i % p == 0) {
                mu[ip] = 0;
                break;
            } else {
                mu[ip] = -mu[i];
            }
        }
    }

    /* fill a_coeff array */
    for (idx_t j = 0; j <= limit; ++j) {
        if (j < 4) {
            a_coeff[j] = 0;
        } else {
            int8_t v1 = (mu[j] == 0) ? 0 : ((mu[j] > 0) ? 1 : 1);
            int8_t v0 = (mu[j-1] == 0) ? 0 : ((mu[j-1] > 0) ? 1 : 1);
            a_coeff[j] = (seq_t)(v1 - v0);
        }
    }

    /* clean up */
    free(is_comp);
    free(mu);
    free(primes);
}

/* compute B[0..limit] using the recursion */
static void compute_B_upto(idx_t limit)
{
    B[0] = 0;
    for (idx_t n = 1; n <= limit; ++n) {
        seq_t sum = 0;
        for (idx_t k = 1; k < n; ++k) {
            sum += B[k] * a_coeff[n - k];
        }
        B[n] = (seq_t)n * a_coeff[n] - sum;
    }
}

/* ensure cache contains values up to n */
static void ensure_capacity(idx_t n)
{
    if (n <= max_n) return;

    /* free previous cache */
    if (a_coeff) free(a_coeff);
    if (B)       free(B);

    /* allocate new arrays */
    a_coeff = (seq_t*)calloc((size_t)n + 1, sizeof(seq_t));
    B       = (seq_t*)calloc((size_t)n + 1, sizeof(seq_t));
    if (!a_coeff || !B) { exit(1); }

    /* compute up to n */
    compute_mu_and_a(n);
    compute_B_upto(n);

    max_n = n;
}

/* public sequence function */
seq_t a(idx_t n)
{
    if (n == 0) return 0;
    ensure_capacity(n);
    return B[n];
}
