/*
 * A355103  E.g.f. A(x) satisfies A(x) = 1 + 3 * x * A(1 - exp(-x)).
 * https://oeis.org/A355103
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign
 * Formula:   a(n) = 3 * n * Σ_{k=0}^{n-1} (-1)^(n-k-1) * S(n-1,k) * a(k) with
 *            a(0) = 1, where S(n,k) are Stirling numbers of the second kind
 * Range:     i8 n <= 2, i16 n <= 5, i32 n <= 9, i64 n <= 15, i128 n <= 18,
 *            big any n
 * Helpers:   seq_alloc_new_array, seq_free_array
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdbool.h>
#include "../utils/seq_alloc_new_array.h"   /* seq_t* seq_alloc_new_array(size_t) */
#include "../utils/seq_free_array.h"        /* void seq_free_array(seq_t*) */

/* Compute the n‑th term of OEIS sequence A355103 */
seq_t a(idx_t n)
{
    /*--- Static tables for computed values --------------------------------*/
    static seq_t *aseq   = NULL;   /* a computed values                   */
    static size_t a_cap  = 0;
    static idx_t  a_max  = 0;       /* highest index already computed      */

    /*--- Static tables for Stirling numbers S(n-1,k)                      */
    static seq_t *Sprev  = NULL;    /* holds S(n-1,k) for current n-1      */
    static size_t S_cap  = 0;
    static idx_t  S_max  = 0;       /* current row index stored in Sprev   */

    /*--- Initial allocation on first call --------------------------------*/
    if (aseq == NULL) {
        a_cap = 1;
        aseq = seq_alloc_new_array(a_cap);
        aseq[0] = (seq_t)1;
        a_max = 0;

        S_cap = 1;
        Sprev = seq_alloc_new_array(S_cap);
        Sprev[0] = (seq_t)1;       /* S(0,0) = 1 */
        S_max = 0;
    }

    /*--- Grow aseq and Sprev if required --------------------------------*/
    if ((size_t)n + 1 > a_cap) {
        size_t new_a_cap = a_cap * 2;
        if (new_a_cap < (size_t)n + 1) new_a_cap = (size_t)n + 1;
        seq_t *new_a = seq_alloc_new_array(new_a_cap);
        for (size_t i = 0; i <= a_max; ++i) new_a[i] = aseq[i];
        seq_free_array(aseq);
        aseq = new_a;
        a_cap = new_a_cap;
    }
    if ((size_t)n + 1 > S_cap) {
        size_t new_S_cap = S_cap * 2;
        if (new_S_cap < (size_t)n + 1) new_S_cap = (size_t)n + 1;
        seq_t *new_S = seq_alloc_new_array(new_S_cap);
        for (size_t i = 0; i <= S_max; ++i) new_S[i] = Sprev[i];
        /* initialise remaining entries to 0 */
        for (size_t i = S_max + 1; i < new_S_cap; ++i) new_S[i] = (seq_t)0;
        seq_free_array(Sprev);
        Sprev = new_S;
        S_cap = new_S_cap;
    }

    /*--- Compute missing values up to n ---------------------------------*/
    for (idx_t cur = a_max + 1; cur <= n; ++cur) {
        if (cur == 0) {  /* a(0) already set */
            aseq[0] = (seq_t)1;
            continue;
        }

        /* Compute Σ term using Sprev (which holds S(cur-1, k))          */
        seq_t sum = (seq_t)0;
        for (idx_t k = 0; k < cur; ++k) {
            seq_t prod = Sprev[(size_t)k] * aseq[(size_t)k];
            /* (-1)^(cur-k-1) */
            if (((cur - k - 1) & 1) != 0)
                sum -= prod;
            else
                sum += prod;
        }

        seq_t factor = (seq_t)3 * (seq_t)cur;
        aseq[(size_t)cur] = factor * sum;

        /* Update Stirling numbers to next row: compute S(cur, k)      */
        /* Start from the end to avoid overwriting needed values        */
        for (idx_t k = cur; k >= 1; --k) {
            seq_t val = Sprev[(size_t)k - 1] + (seq_t)k * Sprev[(size_t)k];
            Sprev[(size_t)k] = val;
        }
        Sprev[0] = (seq_t)0;   /* S(n,0) = 0 for n>0   */
    }

    a_max = n;
    S_max = n;   /* now Sprev holds S(n,·), but next calculation will need S(n,·)
                    as S(prev) for n+1                          */
    return aseq[(size_t)n];
}
