/*
 * A352073  Expansion of e.g.f. 1/(1 - log(1 + 4*x))^(1/4).
 * https://oeis.org/A352073
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign
 * Formula:   a(n) = Sum_{k=0..n} 4^(n-k) * (Product_{j=0..k-1} (4*j+1)) *
 *            Stirling1(n,k)
 * Range:     i8 n <= 4, i16 n <= 6, i32 n <= 9, i64 n <= 15, i128 n <= 18,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdbool.h>
#ifdef __cplusplus
#include <new>
#endif

seq_t a(idx_t n)
{
    /* static state: stores pre–computed values */
    static seq_t *a_vals      = NULL;
    static seq_t *pow4_vals   = NULL;
    static seq_t *P_vals      = NULL;
    static seq_t *s_prev_arr  = NULL;
    static seq_t *s_cur_arr   = NULL;
    static size_t  cap        = 0;          /* current allocation capacity   */
    static bool    init       = false;      /* first call so nothing allocated yet */
    static idx_t   last_computed = 0;       /* largest n already computed    */
    static size_t  pow_len  = 0;           /* how many values of pow4 are known  */
    static size_t  P_len    = 0;           /* how many values of P    are known  */

    size_t target = (size_t)n;             /* target index (0‑based) in size_t form */

    /* --- One‑time initialization --------------------------------------- */
    if (!init)
    {
        cap = 1;
        a_vals      = (seq_t*)malloc(cap * sizeof(seq_t));
        pow4_vals   = (seq_t*)malloc(cap * sizeof(seq_t));
        P_vals      = (seq_t*)malloc(cap * sizeof(seq_t));
        s_prev_arr  = (seq_t*)malloc(cap * sizeof(seq_t));
        s_cur_arr   = (seq_t*)malloc(cap * sizeof(seq_t));

#ifdef __cplusplus
        for (size_t i = 0; i < cap; ++i)
        {
            new (a_vals     + i) seq_t();
            new (pow4_vals  + i) seq_t();
            new (P_vals     + i) seq_t();
            new (s_prev_arr + i) seq_t();
            new (s_cur_arr  + i) seq_t();
        }
#endif

        /* base values */
        a_vals[0]   = (seq_t)1;
        pow4_vals[0] = (seq_t)1;
        P_vals[0]   = (seq_t)1;
        s_prev_arr[0] = (seq_t)1;   /* s(0,0) = 1 */

        last_computed = 0;
        pow_len = 1;
        P_len   = 1;
        init    = true;
    }

    /* --- Increase capacity if needed ----------------------------------- */
    if (target + 1 > cap)
    {
        size_t new_cap = cap;
        if (new_cap == 0) new_cap = 1;
        while (new_cap <= target) new_cap <<= 1;

        a_vals      = (seq_t*)realloc(a_vals,      new_cap * sizeof(seq_t));
        pow4_vals   = (seq_t*)realloc(pow4_vals,   new_cap * sizeof(seq_t));
        P_vals      = (seq_t*)realloc(P_vals,      new_cap * sizeof(seq_t));
        s_prev_arr  = (seq_t*)realloc(s_prev_arr,  new_cap * sizeof(seq_t));
        s_cur_arr   = (seq_t*)realloc(s_cur_arr,   new_cap * sizeof(seq_t));

#ifdef __cplusplus
        for (size_t i = cap; i < new_cap; ++i)
        {
            new (a_vals     + i) seq_t();
            new (pow4_vals  + i) seq_t();
            new (P_vals     + i) seq_t();
            new (s_prev_arr + i) seq_t();
            new (s_cur_arr  + i) seq_t();
        }
#endif
        cap = new_cap;
    }

    /* --- Pre‑compute auxiliary tables up to target --------------------- */
    /* pow4[i] = 4^i */
    for (size_t i = pow_len; i <= target; ++i)
    {
        if (i == 0) { pow4_vals[0] = (seq_t)1; }
        else        { pow4_vals[i] = pow4_vals[i-1] * (seq_t)4; }
    }
    pow_len = target + 1;

    /* P[i] = product_{j=0}^{i-1} (4*j+1)  (P[0]=1) */
    for (size_t i = P_len; i <= target; ++i)
    {
        if (i == 0) { P_vals[0] = (seq_t)1; }
        else
        {
            seq_t mult = (seq_t)(4 * (i-1) + 1);
            P_vals[i] = P_vals[i-1] * mult;
        }
    }
    P_len = target + 1;

    /* --- Compute Stirling numbers and sequence entries for missing n  --- */
    for (idx_t nn = last_computed + 1; nn <= target; ++nn)
    {
        /* 1. compute s_cur for current n   */
        s_cur_arr[0] = (seq_t)0;
        for (size_t k = 1; k <= (size_t)nn; ++k)
        {
            seq_t t = s_prev_arr[k];
            t *= (seq_t)(nn - 1);              /* multiply by (n-1) */
            s_cur_arr[k] = s_prev_arr[k-1] - t;
        }

        /* 2. compute a[nn] via the formula */
        seq_t sum = (seq_t)0;
        for (size_t k = 0; k <= (size_t)nn; ++k)
        {
            seq_t term = s_cur_arr[k];
            term *= P_vals[k];
            term *= pow4_vals[nn - k];
            sum += term;
        }
        a_vals[nn] = sum;

        /* 3. prepare for next iteration (swap arrays) */
        seq_t *tmp = s_prev_arr;
        s_prev_arr = s_cur_arr;
        s_cur_arr  = tmp;
    }

    last_computed = target;
    return a_vals[target];
}
