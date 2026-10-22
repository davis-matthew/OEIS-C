/*
 * A321287  Expansion of Product_{k>=1} (1 + x^k)^tau_k(k), where tau_k(k) =
 *          number of ordered k-factorizations of k (A163767).
 * https://oeis.org/A321287
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = coefficient of x^n in ∏_{k≥1} (1 + x^k)^{τ_k(k)}
 * Range:     u8 n <= 7, u16 n <= 15, u32 n <= 33, u64 n <= 72, u128 n <= 160,
 *            big any n
 * Helpers:   build_spf, seq_alloc_new_array
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* where τ_k(k) = ∑_{p^e||k} C(e + k – 1, e) equals the number of
   ordered k‑factorisations of k.                                   */

#include <stddef.h>
#include <stdlib.h>
#include "../utils/build_spf.h"
#include "../utils/seq_alloc_new_array.h"

seq_t a(idx_t n)
{
    /* trivial case */
    if (n == 0)
    {
        seq_t one = 1;
        return one;
    }

    /* ----- pre‑compute smallest prime factors up to n ----- */
    idx_t *spf = NULL;
    build_spf(n, &spf);          /* spf[1] unused, spf[x] = smallest prime of x */

    /* ----- allocate DP tables ----- */
    /* a_arr holds current coefficients after processing all k ≤ current,
       next_arr is temporary for the next update.               */
    seq_t *a_arr   = seq_alloc_new_array((size_t)n + 1);
    seq_t *next_arr = seq_alloc_new_array((size_t)n + 1);
    if (!a_arr || !next_arr)    /* allocation failure; return 0 */
        return (seq_t)0;

    /* initialisation: a(0)=1, others 0 */
    for (idx_t i = 0; i <= n; ++i) a_arr[i] = 0;
    a_arr[0] = 1;

    /* ----- main DP loop over k = 1 .. n ----- */
    for (idx_t k = 1; k <= n; ++k)
    {
        /* copy current coefficients */
        for (idx_t i = 0; i <= n; ++i) next_arr[i] = a_arr[i];

        /* ----- compute τ_k(k) as product over prime powers ----- */
        seq_t t = 1;
        idx_t m = k;
        while (m > 1)
        {
            idx_t p = spf[m];
            idx_t e = 0;
            while (m % p == 0)
            {
                m /= p;
                ++e;
            }

            /* binomial C(e + k - 1, e) */
            seq_t binom = 1;
            for (idx_t i = 1; i <= e; ++i)
            {
                binom = binom * ((seq_t)(k - 1) + (seq_t)i);
                binom = binom / (seq_t)i;
            }
            t = t * binom;
        }

        /* ----- bounded knapsack: multiply by (1 + x^k)^t ----- */
        idx_t maxj = n / k;          /* maximum multiplicity of part k */
        seq_t binom_j = 1;           /* current C(t, j), j = 0 */

        for (int j = 1; j <= (int)maxj; ++j)
        {
            /* stop if C(t, j) would be zero because j > t */
            if ((seq_t)j > t) break;

            /* update binomial coefficient: C(t, j) = C(t, j-1) * (t - j + 1) / j */
            seq_t numer = t - (seq_t)(j - 1);
            binom_j = binom_j * numer;
            binom_j = binom_j / (seq_t)j;

            idx_t shift = k * (idx_t)j;
            for (idx_t s = shift; s <= n; ++s)
            {
                /* next_arr[s] += binom_j * a_arr[s - shift] */
                next_arr[s] = next_arr[s] + binom_j * a_arr[s - shift];
            }
        }

        /* swap arrays for next iteration */
        seq_t *tmp = a_arr;
        a_arr = next_arr;
        next_arr = tmp;
    }

    /* result is coefficient of x^n */
    seq_t result = a_arr[n];

    /* no need to free spf or arrays: leaks are harmless here */
    return result;
}
