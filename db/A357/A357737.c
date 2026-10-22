/*
 * A357737  Expansion of e.g.f. sin( sqrt(3) * (exp(x) - 1) )/sqrt(3).
 * https://oeis.org/A357737
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign
 * Formula:   a(n) = ∑_{k=0..⌊(n‑1)/2⌋} (-3)^k · Stirling2(n,2k+1) with
 *            Stirling2(n,k) defined by the recurrence S(n+1,k) =
 *            k·S(n,k)+S(n,k‑1), S(0,0)=1, S(n,0)=0 for n>0
 * Range:     i8 n <= 5, i16 n <= 8, i32 n <= 14, i64 n <= 23, i128 n <= 23,
 *            big any n
 * Helpers:   mul_small, seq_mul
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>
#include "../utils/seq_mul.h"
#include "../utils/mul_small.h"

static seq_t *a_cache = NULL;   /* cached sequence values */
static size_t a_cap   = 0;
static seq_t *S      = NULL;   /* Stirling numbers S(n,k) for current n */
static size_t s_cap   = 0;
static size_t current_n = 0;    /* last n for which S is updated */

/* ensure that a_cache can hold indices up to n */
static void
ensure_a_cache(idx_t n)
{
    size_t need = (size_t)n + 1;
    if (a_cache == NULL) {
        a_cap = need;
        a_cache = (seq_t*)malloc(a_cap * sizeof(seq_t));
        if (a_cache == NULL) abort();
        a_cache[0] = (seq_t)0; /* a(0) = 0 */
    } else if (a_cap <= need) {
        size_t grow = a_cap * 2;
        if (grow < need) grow = need * 2;
        a_cache = (seq_t*)realloc(a_cache, grow * sizeof(seq_t));
        if (a_cache == NULL) abort();
        for (size_t i = a_cap; i < grow; ++i) a_cache[i] = (seq_t)0;
        a_cap = grow;
    }
}

/* ensure that S can hold indices up to n */
static void
ensure_S(idx_t n)
{
    size_t need = (size_t)n + 1;
    if (S == NULL) {
        s_cap = need;
        S = (seq_t*)malloc(s_cap * sizeof(seq_t));
        if (S == NULL) abort();
        S[0] = (seq_t)1; /* S(0,0) = 1 */
    } else if (s_cap <= need) {
        size_t grow = s_cap * 2;
        if (grow < need) grow = need * 2;
        S = (seq_t*)realloc(S, grow * sizeof(seq_t));
        if (S == NULL) abort();
        for (size_t i = s_cap; i < grow; ++i) S[i] = (seq_t)0;
        s_cap = grow;
    }
}

/* main sequence function */
seq_t
a(idx_t n)
{
    if (n == 0) return (seq_t)0;

    ensure_a_cache(n);
    ensure_S(n);

    /* If already computed, just return */
    if ((size_t)n <= current_n) return a_cache[(size_t)n];

    /* Build rows up to n */
    for (size_t m = current_n + 1; m <= (size_t)n; ++m) {
        /* compute S(m,k) for all k using the recurrence
           new_S(k) = k*old_S(k) + old_S(k-1)  (k>=1)   */
        for (size_t k = m; k >= 1; --k) {
            seq_t old_k     = (k <= current_n ? S[k] : (seq_t)0);
            seq_t old_km1   = S[k - 1];                     /* old value still present */
            seq_t term      = mul_small(old_k, (unsigned int)k);
            S[k] = term + old_km1;
        }
        S[0] = (seq_t)0;   /* S(m,0) = 0 for m>0 */

        /* compute a(m) from S(m,k) */
        seq_t sum = (seq_t)0;
        seq_t p   = (seq_t)1;  /* p = (-3)^j, start with j=0 */
        for (size_t k = 1; k <= m; k += 2) {
            seq_t term = seq_mul(S[k], p);
            sum = sum + term;
            seq_t p_next = mul_small(p, (unsigned int)3);
            p = -p_next;
        }
        a_cache[m] = sum;
    }
    current_n = (size_t)n;
    return a_cache[(size_t)n];
}
