/*
 * A383261  Expansion of e.g.f. f(x) * exp(2 * f(x)), where f(x) = (exp(3*x) -
 *          1)/3.
 * https://oeis.org/A383261
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Sum_{k=1..n} k*2^(k-1)*3^(n-k)*Stirling2(n,k)
 * Range:     u8 n <= 3, u16 n <= 6, u32 n <= 10, u64 n <= 17, u128 n <= 20,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*           a(0) = 0                                       */

#include <stddef.h>
#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>

/* --------- global state -------------------------------------------------- */
static seq_t *S = NULL;       /* current Stirling numbers S(cur,k)   */
static seq_t *pow2 = NULL;    /* 2^i                                 */
static seq_t *pow3 = NULL;    /* 3^i                                 */
static seq_t *factor = NULL;  /* k*2^(k-1)                           */
static seq_t *cache = NULL;   /* a(n) for n up to computed_max       */
static size_t S_cap = 0;
static size_t pow2_cap = 0;
static size_t pow3_cap = 0;
static size_t factor_cap = 0;
static size_t cache_cap = 0;
static idx_t computed_max = 0;    /* largest n whose a(n) is stored */

/* --------- helper: resize an array --------------------------------------- */
static seq_t *grow_array(seq_t *old, size_t old_cap, size_t new_cap)
{
    seq_t *new_arr = (seq_t *)realloc(old, new_cap * sizeof(seq_t));
    if (!new_arr) abort();          /* fatal error on allocation failure */
    for (size_t i = old_cap; i < new_cap; ++i)
        new_arr[i] = (seq_t)0;      /* initialise new elements to zero   */
    return new_arr;
}

/* --------- ensure the auxiliary arrays are large enough ------------------ */
static void ensure_size(idx_t n)
{
    size_t need = (size_t)n + 1;                 /* include index n */

    if (S_cap < need) {
        S = grow_array(S, S_cap, need);
        S_cap = need;
    }

    if (pow2_cap < need) {
        pow2 = grow_array(pow2, pow2_cap, need);
        if (pow2_cap == 0)
            pow2[0] = (seq_t)1;
        for (size_t i = pow2_cap == 0 ? 1 : pow2_cap; i < need; ++i)
            pow2[i] = pow2[i - 1] * (seq_t)2;
        pow2_cap = need;
    }

    if (pow3_cap < need) {
        pow3 = grow_array(pow3, pow3_cap, need);
        if (pow3_cap == 0)
            pow3[0] = (seq_t)1;
        for (size_t i = pow3_cap == 0 ? 1 : pow3_cap; i < need; ++i)
            pow3[i] = pow3[i - 1] * (seq_t)3;
        pow3_cap = need;
    }

    if (factor_cap < need) {
        factor = grow_array(factor, factor_cap, need);
        for (size_t i = factor_cap; i < need; ++i) {
            if (i == 0)
                factor[i] = (seq_t)0;
            else
                factor[i] = (seq_t)i * pow2[i - 1];
        }
        factor_cap = need;
    }

    if (cache_cap < need) {
        cache = grow_array(cache, cache_cap, need);
        cache_cap = need;
    }
}

/* --------- compute all a(0..n) that are not yet known --------------------- */
static void ensure_computed(idx_t n)
{
    if (computed_max >= n)
        return;                                   /* nothing to do */

    ensure_size(n);

    /* initialise for n = 0 if this is the first computation */
    if (computed_max == 0) {
        S[0] = (seq_t)1;            /* S(0,0) = 1    */
        cache[0] = (seq_t)0;        /* a(0) = 0      */
    }

    for (idx_t cur = computed_max + 1; cur <= n; ++cur) {
        /* update Stirling numbers S(cur,k) in place:      */
        /*   S(cur,k) = k*S(cur-1,k) + S(cur-1,k-1)        */
        for (idx_t k = cur; k >= 1; --k) {
            seq_t val = S[k];                    /* old S(cur-1,k)  */
            val = ((seq_t)k) * val + S[k - 1];   /* use old S(k-1)   */
            S[k] = val;
        }
        S[0] = (seq_t)0;         /* S(cur,0) = 0 for cur>0 */

        /* compute a(cur) using the formula                 */
        if (sizeof(seq_t) <= 8) {        /* seq_t is 64‑bit or less   */
            __uint128_t sum128 = 0;
            for (idx_t k = 1; k <= cur; ++k) {
                __uint128_t term = (__uint128_t)S[k];
                term *= (__uint128_t)factor[k];
                term *= (__uint128_t)pow3[(size_t)cur - k];
                sum128 += term;
            }
            cache[cur] = (seq_t)sum128;
        } else {                         /* arbitrary‑precision build   */
            seq_t sum = (seq_t)0;
            for (idx_t k = 1; k <= cur; ++k) {
                seq_t term = S[k];
                term *= factor[k];
                term *= pow3[(size_t)cur - k];
                sum += term;
            }
            cache[cur] = sum;
        }
    }
    computed_max = n;
}

/* --------- public routine ------------------------------------------------- */
seq_t a(idx_t n)
{
    if (n == 0)
        return (seq_t)0;          /* a(0) defined as 0 */
    ensure_computed(n);
    return cache[n];
}
