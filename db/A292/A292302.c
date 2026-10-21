/*
 * A292302  Expansion of (1 - x)*Sum_{k>=1} k*phi(k)*x^k/(1 - x^k), where
 *          phi() is the Euler totient function (A000010).
 * https://oeis.org/A292302
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  sign
 * Formula:   a(n) = (1 – x) * Sum_{k>=1} k*phi(k)*x^k/(1 - x^k) Expanded,
 *            a(n) = b(n) – b(n-1), where b(n) = Sum_{d|n} d*phi(d), phi is
 *            Euler's totient function. The function a(n) returns this value
 *            for n >= 1, with a(1) = 1. The implementation keeps the values
 *            of phi and b in static arrays and expands them o...
 * Range:     i8 n <= 18, i16 n <= 60, i32 n <= 60, i64 n <= 60, i128 n <= 60,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>

/* external types provided by the build */
typedef uint64_t idx_t;

/* ----- Static cached data ------------------------------------------- */
static seq_t *b = NULL;            /* b[n] = Sum_{d|n} d*phi(d) */
static idx_t b_cap = 0;            /* current capacity (>= 1) */
static idx_t *phi = NULL;          /* phi[n] = Euler's totient of n */
static idx_t phi_cap = 0;          /* current capacity for phi */

/* ---- helper: compute phi[1..N] with a simple sieve ---------------- */
static void compute_phi(idx_t N)
{
    /* allocate/resize phi array */
    idx_t *new_phi = (idx_t *)realloc(phi, (N + 1) * sizeof(idx_t));
    if (!new_phi) exit(EXIT_FAILURE);
    phi = new_phi;
    phi_cap = N + 1;

    for (idx_t i = 0; i <= N; ++i) phi[i] = i;   /* init to i */
    for (idx_t i = 2; i <= N; ++i) {
        if (phi[i] == i) {     /* i is prime */
            for (idx_t j = i; j <= N; j += i) {
                phi[j] = (phi[j] / i) * (i - 1);
            }
        }
    }
    phi[0] = 0;   /* not used but defined for completeness */
    phi[1] = 1;
}

/* ---- helper: recompute b[0..N] ------------------------------------- */
static void recompute_b(idx_t N)
{
    /* allocate/resize b array */
    seq_t *new_b = (seq_t *)realloc(b, (N + 1) * sizeof(seq_t));
    if (!new_b) exit(EXIT_FAILURE);
    b = new_b;
    b_cap = N + 1;

    /* zero it */
    for (idx_t i = 0; i <= N; ++i) b[i] = 0;

    /* compute multiplicative contribution f(d) = d*phi(d) */
    for (idx_t d = 1; d <= N; ++d) {
        seq_t f = (seq_t)d * (seq_t)phi[d];
        for (idx_t m = d; m <= N; m += d) {
            b[m] += f;
        }
    }
}

/* ---- helper: ensure data available up to n ------------------------ */
static void ensure_upto(idx_t n)
{
    if (n >= b_cap) {          /* need more data */
        compute_phi(n);
        recompute_b(n);
    }
}

/* ---- the required function ---------------------------------------- */
seq_t a(idx_t n)
{
    if (n == 0) return 0;      /* undefined for n<1, but harmless */
    ensure_upto(n);
    return b[n] - b[n - 1];
}
