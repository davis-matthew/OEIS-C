/*
 * A357088  E.g.f. satisfies A(x) * log(A(x)) = (exp(x*A(x)) - 1)^2 / 2.
 * https://oeis.org/A357088
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = ∑_{k=0..⌊n/2⌋} (2*k)! * (n-k+1)^{k-1} * Stirling2(n,2*k)
 *            / (2^k * k!) (OEIS A357088)
 * Range:     u8 n <= 5, u16 n <= 7, u32 n <= 11, u64 n <= 18, u128 n <= 20,
 *            big any n
 * Helpers:   big_pow, pow2
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>
#include "../utils/big_pow.h"
#include "../utils/pow2.h"

/* ---------- Internal state ---------- */
static seq_t *a_cache     = NULL;   /* a[0..] values, 0 if not computed */
static size_t a_len       = 0;      /* allocated length of a_cache      */
static size_t a_max_comp  = 0;      /* highest index computed so far    */

static seq_t *fact_cache  = NULL;   /* factorials up to 2*max_n          */
static size_t fact_len    = 0;      /* allocated length of fact_cache   */
static size_t fact_max    = 0;      /* highest factorial computed       */

static seq_t *stir_prev   = NULL;   /* row n-1 of Stirling numbers      */
static seq_t *stir_curr   = NULL;   /* row n   of Stirling numbers      */
static size_t stir_len    = 0;      /* allocated length of stir arrays  */

/* ---------- Utility: grow arrays safely ---------- */
static void ensure_a_capacity(idx_t needed)
{
    if ((size_t)needed < a_len) return;
    size_t new_len = ((size_t)needed + 1) * 2;
    seq_t *tmp = NULL;
    if (a_cache) tmp = (seq_t *)realloc(a_cache, new_len * sizeof(seq_t));
    else          tmp = (seq_t *)malloc(new_len * sizeof(seq_t));
    if (!tmp) abort();
    for (size_t i = a_len; i < new_len; ++i) tmp[i] = 0;
    a_cache = tmp;
    a_len   = new_len;
}

static void ensure_fact_capacity(idx_t needed)
{
    if ((size_t)needed < fact_len) return;
    size_t new_len = ((size_t)needed + 1) * 2;
    seq_t *tmp = NULL;
    if (fact_cache) tmp = (seq_t *)realloc(fact_cache, new_len * sizeof(seq_t));
    else            tmp = (seq_t *)malloc(new_len * sizeof(seq_t));
    if (!tmp) abort();
    for (size_t i = fact_len; i < new_len; ++i) tmp[i] = 0;
    fact_cache = tmp;
    fact_len   = new_len;

    /* compute new factorials */
    for (size_t i = fact_max + 1; i <= (size_t)needed; ++i) {
        if (i == 0) fact_cache[i] = 1;
        else        fact_cache[i] = fact_cache[i-1] * (seq_t)i;
    }
    fact_max = needed;
}

static void ensure_stir_capacity(idx_t needed)
{
    if ((size_t)needed < stir_len) return;
    size_t new_len = ((size_t)needed + 1) * 2;
    seq_t *tmp0 = NULL, *tmp1 = NULL;
    if (stir_prev) tmp0 = (seq_t *)realloc(stir_prev, new_len * sizeof(seq_t));
    else           tmp0 = (seq_t *)malloc(new_len * sizeof(seq_t));
    if (!tmp0) abort();

    if (stir_curr) tmp1 = (seq_t *)realloc(stir_curr, new_len * sizeof(seq_t));
    else           tmp1 = (seq_t *)malloc(new_len * sizeof(seq_t));
    if (!tmp1) abort();

    for (size_t i = stir_len; i < new_len; ++i) {
        tmp0[i] = 0;
        tmp1[i] = 0;
    }
    stir_prev = tmp0;
    stir_curr = tmp1;
    stir_len  = new_len;
}

/* ---------- Main entry function ---------- */
seq_t a(idx_t n)
{
    /* Handle n==0 specially */
    if (n == 0) {
        ensure_a_capacity(0);
        if (a_cache[0] == 0) a_cache[0] = 1;
        a_max_comp = 0;
        return a_cache[0];
    }

    /* Compute values up to n if necessary */
    if ((size_t)n > a_max_comp) {
        /* initialise factorial cache once */
        if (fact_cache == NULL) {
            fact_len = 1;
            fact_cache = (seq_t *)malloc(fact_len * sizeof(seq_t));
            if (!fact_cache) abort();
            fact_cache[0] = 1;
            fact_max = 0;
        }

        /* initialise Stirling arrays once */
        if (stir_prev == NULL) {
            stir_len = 1;
            stir_prev = (seq_t *)malloc(stir_len * sizeof(seq_t));
            stir_curr = (seq_t *)malloc(stir_len * sizeof(seq_t));
            if (!stir_prev || !stir_curr) abort();
            stir_prev[0] = 1;   /* S(0,0)=1 */
        }

        for (idx_t i = a_max_comp + 1; i <= n; ++i) {
            /* extend caches to needed size */
            ensure_a_capacity(i);
            ensure_fact_capacity(2 * i);
            ensure_stir_capacity(i);

            /* compute Stirling row i from previous row */
            stir_curr[0] = 0;
            for (idx_t k = 1; k <= i; ++k) {
                seq_t term1 = stir_prev[k-1];
                seq_t term2 = stir_prev[k] * (seq_t)k;
                stir_curr[k] = term1 + term2;
            }

            /* compute a(i) using 128‑bit intermediate arithmetic */
            size_t maxk = i / 2;
            unsigned __int128 sum = 0;
            for (size_t k = 1; k <= maxk; ++k) {
                seq_t fact_2k   = fact_cache[2*k];
                seq_t base      = (seq_t)(i - k + 1);
                seq_t power     = big_pow(base, (seq_t)(k-1));
                seq_t stir_val  = stir_curr[2*k];

                unsigned __int128 numer   = (unsigned __int128)fact_2k
                                            * (unsigned __int128)power
                                            * (unsigned __int128)stir_val;
                unsigned __int128 denom   = (unsigned __int128)pow2(k)
                                            * (unsigned __int128)fact_cache[k];
                unsigned __int128 term    = numer / denom;
                sum += term;
            }
            a_cache[i] = (seq_t)sum;

            /* swap rows for next iteration */
            seq_t *tmp = stir_prev;
            stir_prev = stir_curr;
            stir_curr = tmp;
        }
        a_max_comp = n;
    }

    return a_cache[n];
}
