/*
 * A295832  Expansion of Product_{k>=1} ((1 + x^(2*k-1))/(1 - x^(2*k)))^k.
 * https://oeis.org/A295832
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 12, u16 n <= 28, u32 n <= 45, u64 n <= 45, u128 n <= 45,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* 
 *  OEIS A295832 – Expansion of ∏_{k≥1} ((1 + x^(2k-1))/(1 - x^(2k)))^k
 *
 *  The ordinary generating function G(x) satisfies
 *    G(x) = exp( Σ_{m≥1} E_m x^m )
 *
 *  with
 *    E_m = Σ_{k≥1} k * ( (-1)^(q+1)/q  if m = q(2k−1)  or
 *                      1/q           if m = q(2k)   )
 *
 *  Let K(n) = n * E_n.  Then K(n) can be computed by
 *
 *      K(n) = Σ_{t odd, t|n} ((t+1)*t)/2 * ( (-1)^(n/t+1) )
 *           + Σ_{k ≥ 1, 2k|n} 2k²
 *
 *  The coefficients a(n) of G(x) obey the recurrence
 *
 *      a(0) = 1
 *      a(n) = (1/n) * Σ_{k=1..n} K(k) * a(n-k)   for n ≥ 1
 *
 *  The following implementation memoises a(n) and K(n) up to the
 *  requested index, using only integer arithmetic.
 */

#include <stdlib.h>
#include <stdint.h>

seq_t a(idx_t n)
{
    /* static memoised tables */
    static seq_t  *seq   = NULL;   /* a[0..max_n]                 */
    static idx_t   max_n = 0;      /* highest index computed for a */
    static int64_t *Karr  = NULL;  /* K[1..max_K]                  */
    static idx_t   max_K = 0;      /* highest index computed for K */

    /* first term */
    if (n == 0u)
        return (seq_t)1;

    /* enlarge tables if necessary */
    if (n > max_n) {
        idx_t newmax = n;

        /* allocate or grow seq array */
        seq = (seq_t *)realloc(seq, (size_t)(newmax + 1) * sizeof(seq_t));

        /* allocate or grow K array */
        Karr = (int64_t *)realloc(Karr, (size_t)(newmax + 1) * sizeof(int64_t));

        /* initialise seq[0] */
        if (max_n == 0u)
            seq[0] = (seq_t)1;

        /* compute K and a for each new index */
        for (idx_t i = max_n + 1; i <= newmax; ++i) {
            /* --- compute K[i] --------------------------------------------- */
            int64_t k_val = 0;

            /* contribution from terms with m(2k):  Σ_{k:2k|i} 2k² */
            for (idx_t k = 1; 2*k <= i; ++k) {
                if (i % (2*k) == 0)
                    k_val += 2LL * (int64_t)k * (int64_t)k;
            }

            /* contribution from terms with m(2k-1): Σ_{t odd|i} ((t+1)t)/2 * (-1)^(i/t+1) */
            for (idx_t t = 1; t <= i; t += 2) {
                if (i % t == 0) {
                    idx_t m   = i / t;
                    int    sign = (m & 1) ? 1 : -1;    /* (-1)^(m+1) */
                    int64_t term = (int64_t)((t + 1) * t) / 2;
                    k_val += sign * term;
                }
            }

            Karr[i] = k_val;

            /* --- compute a[i] --------------------------------------------- */
            __int128 sum = 0;
            for (idx_t k = 1; k <= i; ++k) {
                /* multiply signed Karr[k] with a[i-k] (unsigned) */
                sum += ( (__int128)Karr[k] ) * ( (__int128)seq[i - k] );
            }
            seq[i] = (seq_t)(sum / i);   /* division is exact by theory */
        }

        max_n = newmax;
        max_K = newmax;
    }

    return seq[n];
}
