/*
 * A282780  Coefficients in q-expansion of E_2^3*E_6, where E_2 and E_6 are
 *          respectively the Eisenstein series A006352 and A013973.
 * https://oeis.org/A282780
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign
 * Formula:   a(n) = coefficient of q^n in E_2(q)^3 * E_6(q) where E_2(q) = 1
 *            - 24 Σ_{m≥1} σ_1(m) q^m (σ_1(m) = sum of divisors of m) E_6(q) =
 *            1 - 504 Σ_{m≥1} σ_5(m) q^m (σ_5(m) = Σ d^5 over divisors d|m)
 * Range:     i8 n <= 0, i16 n <= 2, i32 n <= 6, i64 n <= 35, i128 n <= 1000,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

static seq_t *E2   = NULL;   /* coefficients of E_2  */
static seq_t *E6   = NULL;   /* coefficients of E_6  */
static seq_t *Sq   = NULL;   /* E_2^2 coefficients */
static seq_t *Cub  = NULL;   /* E_2^3 coefficients */
static seq_t *Ans  = NULL;   /* final coefficients a(n) */
static idx_t  maxN = 0;      /* highest n for which arrays are valid */

/* Allocate or resize working arrays up to N and fill them */
static void compute_up_to(idx_t N)
{
    size_t sz = (size_t)N + 1;

    /* free old storage */
    free(E2);   free(E6);   free(Sq);   free(Cub);   free(Ans);
    /* allocate zero‑initialised arrays */
    E2   = (seq_t *)calloc(sz, sizeof(seq_t));
    E6   = (seq_t *)calloc(sz, sizeof(seq_t));
    Sq   = (seq_t *)calloc(sz, sizeof(seq_t));
    Cub  = (seq_t *)calloc(sz, sizeof(seq_t));
    Ans  = (seq_t *)calloc(sz, sizeof(seq_t));

    /* never forget the constant term of E_6 (and E_2) */
    E2[0] = 1;
    E6[0] = 1;

    /* --- compute σ₁(n) and σ₅(n) and fill E_2, E_6 --------------------- */
    for (idx_t n = 1; n <= N; ++n)
    {
        seq_t s1 = 0;   /* σ₁(n) */
        seq_t s5 = 0;   /* σ₅(n) */
        for (idx_t d = 1; d <= n / d; ++d)
        {
            if (n % d == 0)
            {
                seq_t dd   = (seq_t)d;
                seq_t d2   = dd * dd;
                seq_t d3   = d2 * dd;
                seq_t d5   = d3 * d2;   /* d⁵ = d³·d² */

                s1 += dd;
                s5 += d5;

                if (d != n / d)
                {
                    seq_t dd2  = (seq_t)(n / d);
                    seq_t d22  = dd2 * dd2;
                    seq_t d32  = d22 * dd2;
                    seq_t d52  = d32 * d22;   /* (n/d)⁵ */

                    s1 += dd2;
                    s5 += d52;
                }
            }
        }
        E2[n] = -24 * s1;   /* coefficient of q^n in E_2 */
        E6[n] = -504 * s5;  /* coefficient of q^n in E_6 */
    }

    /* --- convolution: E_2^2 ---------------------------------------------- */
    for (idx_t i = 0; i <= N; ++i)
    {
        for (idx_t j = 0; j <= N - i; ++j)
            Sq[i + j] += E2[i] * E2[j];
    }

    /* --- convolution: E_2^3 ---------------------------------------------- */
    for (idx_t i = 0; i <= N; ++i)
    {
        for (idx_t j = 0; j <= N - i; ++j)
            Cub[i + j] += Sq[i] * E2[j];
    }

    /* --- final convolution: E_2^3 * E_6 --------------------------------- */
    for (idx_t i = 0; i <= N; ++i)
    {
        for (idx_t j = 0; j <= N - i; ++j)
            Ans[i + j] += Cub[i] * E6[j];
    }

    maxN = N;
}

/* Public function required by the build system */
seq_t a(idx_t n)
{
    /* Ensure that arrays are built for at least n (including the case n==0) */
    if (n > maxN || maxN == 0)
        compute_up_to(n);

    return Ans[n];
}
