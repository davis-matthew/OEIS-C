/*
 * A350722  a(n) = Sum_{k=0..n} k! * k^(k+n) * Stirling2(n,k).
 * https://oeis.org/A350722
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Sum_{k=0..n} k! * k^(k+n) * Stirling2(n,k)
 * Range:     u8 n <= 2, u16 n <= 3, u32 n <= 5, u64 n <= 8, u128 n <= 12,
 *            big any n
 * Helpers:   big_pow, seq_alloc_new_array
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>
#include "../utils/big_pow.h"
#include "../utils/seq_alloc_new_array.h"

seq_t a(idx_t n)
{
    /* ---------- static state ---------- */
    static bool   init          = false;      /* tables initialised */
    static idx_t  max_computed  = 0;          /* largest n computed */
    static size_t cap           = 0;          /* array capacity       */

    static seq_t *a_cache       = NULL;       /* a[0..max_computed]   */
    static seq_t *S_prev        = NULL;       /* Stirling row n-1     */
    static seq_t *S_curr        = NULL;       /* Stirling row n       */
    static seq_t *pow_kn        = NULL;       /* k^n for current n   */
    static seq_t *fact_k        = NULL;       /* k!                  */
    static seq_t *kkfact_k      = NULL;       /* k! * k^k             */

    /* ---------- helper to enlarge arrays ---------- */
    if (!init) {
        cap = 4;                            /* minimal capacity */
        a_cache   = seq_alloc_new_array(cap);
        S_prev    = seq_alloc_new_array(cap);
        S_curr    = seq_alloc_new_array(cap);
        pow_kn    = seq_alloc_new_array(cap);
        fact_k    = seq_alloc_new_array(cap);
        kkfact_k  = seq_alloc_new_array(cap);

        /* initialise for n = 0 */
        a_cache[0]    = (seq_t)1;           /* a(0) = 1 */
        fact_k[0]     = (seq_t)1;           /* 0!  = 1 */
        pow_kn[0]     = (seq_t)1;           /* 0^0 = 1 */
        kkfact_k[0]   = (seq_t)1;           /* 0! * 0^0 = 1 */
        S_prev[0]     = (seq_t)1;           /* S(0,0) = 1 */
        S_prev[1]     = (seq_t)0;           /* S(0,1) = 0 */

        /* remaining entries zero */
        for (size_t i = 1; i < cap; ++i) {
            a_cache[i]   = (seq_t)0;
            S_prev[i]    = (seq_t)0;
            S_curr[i]    = (seq_t)0;
            pow_kn[i]    = (seq_t)0;
            fact_k[i]    = (seq_t)0;
            kkfact_k[i]  = (seq_t)0;
        }

        init = true;
    }

    if (n >= cap) {
        size_t new_cap = cap;
        while (new_cap <= (size_t)n) new_cap <<= 1;

        seq_t *tmp;

        tmp = seq_alloc_new_array(new_cap);
        for (size_t i = 0; i < cap; ++i) tmp[i] = a_cache[i];
        free(a_cache);
        a_cache = tmp;

        tmp = seq_alloc_new_array(new_cap);
        for (size_t i = 0; i < cap; ++i) tmp[i] = S_prev[i];
        free(S_prev);
        S_prev = tmp;

        tmp = seq_alloc_new_array(new_cap);
        for (size_t i = 0; i < cap; ++i) tmp[i] = S_curr[i];
        free(S_curr);
        S_curr = tmp;

        tmp = seq_alloc_new_array(new_cap);
        for (size_t i = 0; i < cap; ++i) tmp[i] = pow_kn[i];
        free(pow_kn);
        pow_kn = tmp;

        tmp = seq_alloc_new_array(new_cap);
        for (size_t i = 0; i < cap; ++i) tmp[i] = fact_k[i];
        free(fact_k);
        fact_k = tmp;

        tmp = seq_alloc_new_array(new_cap);
        for (size_t i = 0; i < cap; ++i) tmp[i] = kkfact_k[i];
        free(kkfact_k);
        kkfact_k = tmp;

        cap = new_cap;
    }

    /* ---------- compute a(n) incrementally ---------- */
    for (idx_t cur = max_computed + 1; cur <= n; ++cur) {
        /* factorials and k! * k^k for the new k = cur */
        if (cur > 0) {
            fact_k[(size_t)cur] = fact_k[(size_t)(cur - 1)] * (seq_t)cur;
            seq_t pkk = big_pow((seq_t)cur, (seq_t)cur);
            kkfact_k[(size_t)cur] = fact_k[(size_t)cur] * pkk;
        }

        /* k^n for all k <= cur */
        for (idx_t k = 1; k <= cur; ++k) {
            if (k < cur)                /* old k: raise exponent by 1 */
                pow_kn[(size_t)k] = pow_kn[(size_t)k] * (seq_t)k;
            else                         /* new k = cur: k^cur itself */
                pow_kn[(size_t)k] = big_pow((seq_t)cur, (seq_t)cur);
        }

        /* Stirling numbers of the second kind: row cur */
        S_curr[0] = (seq_t)0;
        if (cur >= 1) S_curr[1] = (seq_t)1;
        for (idx_t k = 2; k <= cur; ++k) {
            seq_t term1 = (seq_t)k * S_prev[(size_t)k];
            seq_t term2 = S_prev[(size_t)(k - 1)];
            S_curr[(size_t)k] = term1 + term2;
        }

        /* compute a[cur] by summing over k = 1..cur */
        seq_t sum = (cur == 0) ? (seq_t)1 : (seq_t)0;
        for (idx_t k = 1; k <= cur; ++k) {
            seq_t term = kkfact_k[(size_t)k];
            term = term * pow_kn[(size_t)k];
            term = term * S_curr[(size_t)k];
            sum = sum + term;
        }
        a_cache[(size_t)cur] = sum;

        /* For next iteration, make this row the previous row */
        seq_t *swap = S_prev;
        S_prev = S_curr;
        S_curr = swap;
    }

    max_computed = n;
    return a_cache[(size_t)n];
}
