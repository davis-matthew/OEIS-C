/*
 * A307656  G.f. A(x) satisfies: (1 - x) = A(x)*A(x^2)^2*A(x^3)^3*A(x^4)^4*
 *          ... *A(x^k)^k* ...
 * https://oeis.org/A307656
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign
 * Formula:   not stated
 * Range:     i8 n <= 6, i16 n <= 27, i32 n <= 50, i64 n <= 50, i128 n <= 50,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*OEIS A307656
 * G.f.: Product_{k≥1} (1 - x^k)^(μ(k)·k)
 * Recurrence: a(0)=1,
 *             a(n)= -(1/n) · Σ_{k=1..n} S_k · a(n−k),   n≥1
 * where S_k = Σ_{d|k} μ(d)·d²
 */

#include <stdlib.h>
#include <stdint.h>
#include <stddef.h>

/* external types provided by the build system */
extern seq_t a(idx_t n);

seq_t a(idx_t n)
{
    /* static cache for computed values */
    static seq_t *A   = NULL;   /* a[0..cap-1]          */
    static int  *MU  = NULL;   /* μ[0..cap-1] (int)    */
    static seq_t *S   = NULL;   /* S[0..cap-1]          */
    static size_t cap = 0;      /* current capacity     */

    /* grow the cache if n is outside current capacity */
    if ((size_t)n >= cap)
    {
        size_t newcap = (cap == 0) ? 1 : (2 * cap);
        if (newcap <= (size_t)n) newcap = (size_t)n + 1;

        /* reallocate arrays */
        A  = (seq_t *)realloc(A, newcap * sizeof(seq_t));
        MU = (int  *)realloc(MU, newcap * sizeof(int));
        S  = (seq_t *)realloc(S, newcap * sizeof(seq_t));
        if (!A || !MU || !S)
            abort();   /* allocation failed */

        /*-----------------------------------------------------------------
         * 1) compute Möbius μ[1..newcap-1] by linear sieve
         *-----------------------------------------------------------------*/
        size_t N = newcap - 1;
        int *is_comp = (int *)calloc(N + 1, sizeof(int));
        int *primes  = (int *)malloc((N + 1) * sizeof(int));
        size_t pcnt = 0;

        MU[0] = 0;          /* unused but defined for safety */
        if (N >= 1) MU[1] = 1;
        for (size_t i = 2; i <= N; ++i)
        {
            if (!is_comp[i])
            {
                primes[pcnt++] = (int)i;
                MU[i] = -1;
            }
            for (size_t pj = 0; pj < pcnt; ++pj)
            {
                size_t p = (size_t)primes[pj];
                size_t ip = i * p;
                if (ip > N) break;
                is_comp[ip] = 1;
                if (i % p == 0)
                {
                    MU[ip] = 0;
                    break;
                }
                else
                {
                    MU[ip] = -MU[i];
                }
            }
        }
        free(is_comp);
        free(primes);

        /*-----------------------------------------------------------------
         * 2) precompute S[k] = Σ_{d|k} μ(d)·d²   for k=1..N
         *-----------------------------------------------------------------*/
        for (size_t i = 0; i <= N; ++i) S[i] = 0;
        for (size_t d = 1; d <= N; ++d)
        {
            int mu_d = MU[d];
            if (mu_d == 0) continue;
            seq_t d2 = (seq_t)d * (seq_t)d;
            seq_t add = (seq_t)mu_d * d2;   /* μ(d)·d² */
            for (size_t m = d; m <= N; m += d)
                S[m] += add;
        }

        /*-----------------------------------------------------------------
         * 3) compute a[n] up to newcap-1
         *-----------------------------------------------------------------*/
        for (size_t i = cap; i < newcap; ++i)
        {
            if (i == 0) { A[0] = 1; continue; }
            seq_t sum = 0;
            for (size_t k = 1; k <= i; ++k)
                sum += S[k] * A[i - k];
            /* division is exact by construction */
            A[i] = - (sum / (seq_t)i);
        }
        cap = newcap;
    }
    return A[(size_t)n];
}
