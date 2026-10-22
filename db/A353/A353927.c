/*
 * A353927  Product_{n>=1} (1 + x^n)^a(n) = 1 + Sum_{n>=1} mu(n)*x^n, where mu
 *          = A008683.
 * https://oeis.org/A353927
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  sign
 * Formula:   a(n) defined by * ∏_{k≥1} (1 + x^k)^{a(k)} = 1 + ∑_{m≥1} μ(m)
 *            x^m * where μ is the Möbius function. * The coefficients a(n)
 *            are obtained via the recurrence below
 * Range:     i8 n <= 14, i16 n <= 25, i32 n <= 47, i64 n <= 47, i128 n <= 47,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include <stdlib.h>

static seq_t *w = NULL;          /* w[n] = n * c(n), c = log-coefficient of RHS */
static seq_t *A = NULL;          /* A[n] = a(n) * n */
static seq_t *a_arr = NULL;      /* final a(n) values */
static int *mu = NULL;           /* Möbius function up to current maximum */
static idx_t current_max = 0;    /* highest n for which arrays are computed */

/* Helper: extend all tables up to at least n.  Complexity is O(n^2) overall. */
static void ensure_arrays(idx_t n)
{
    if (n <= current_max)
        return;

    /* --- compute Möbius function up to n (linear sieve) --- */
    int *new_mu = (int*)realloc(mu, sizeof(int) * (n + 1));
    if (!new_mu) exit(1);
    new_mu[0] = 0;
    new_mu[1] = 1;

    int *primes = (int*)malloc(sizeof(int) * (n + 1));
    bool *is_comp = (bool*)calloc(n + 1, 1);
    int prime_cnt = 0;

    for (idx_t i = 2; i <= n; ++i) {
        if (!is_comp[(size_t)i]) {
            primes[prime_cnt++] = (int)i;
            new_mu[(size_t)i] = -1;
        }
        for (int j = 0; j < prime_cnt && (idx_t)i * primes[j] <= n; ++j) {
            idx_t v = i * primes[j];
            is_comp[(size_t)v] = true;
            if (i % primes[j] == 0) {
                new_mu[(size_t)v] = 0;
                break;
            } else {
                new_mu[(size_t)v] = -new_mu[(size_t)i];
            }
        }
    }
    free(primes);
    free(is_comp);
    mu = new_mu;

    /* --- allocate or reallocate working arrays --- */
    w     = (seq_t*)realloc(w,     sizeof(seq_t) * (n + 1));
    A     = (seq_t*)realloc(A,     sizeof(seq_t) * (n + 1));
    a_arr = (seq_t*)realloc(a_arr, sizeof(seq_t) * (n + 1));

    /* --- compute w[n] for new indices --- */
    for (idx_t t = current_max + 1; t <= n; ++t) {
        seq_t wn = (seq_t)t * (seq_t)mu[(size_t)t];
        for (idx_t k = 1; k < t; ++k) {
            wn = wn - w[(size_t)k] * (seq_t)mu[(size_t)(t - k)];
        }
        w[(size_t)t] = wn;
    }

    /* --- compute A[n] (and hence a[n]) for new indices --- */
    for (idx_t t = current_max + 1; t <= n; ++t) {
        seq_t sum = (seq_t)0;
        for (idx_t d = 1; d * d <= t; ++d) {
            if (t % d == 0) {
                idx_t d1 = d;
                idx_t d2 = t / d;
                if (d1 < t) {
                    seq_t sign = ((t / d1) % 2 == 1) ? (seq_t)1 : (seq_t)-1;
                    sum = sum + A[(size_t)d1] * sign;
                }
                if (d2 != d1 && d2 < t) {
                    seq_t sign = ((t / d2) % 2 == 1) ? (seq_t)1 : (seq_t)-1;
                    sum = sum + A[(size_t)d2] * sign;
                }
            }
        }
        A[(size_t)t] = w[(size_t)t] - sum;
        a_arr[(size_t)t] = A[(size_t)t] / (seq_t)t;
    }

    current_max = n;
}

/* Public API -------------------------------------------------------------- */
seq_t a(idx_t n)
{
    ensure_arrays(n);
    return a_arr[(size_t)n];
}
