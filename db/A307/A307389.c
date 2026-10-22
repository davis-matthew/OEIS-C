/*
 * A307389  a(n) is the number of elements in the species of orbit polytopes
 *          in dimension n.
 * https://oeis.org/A307389
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(0)=1, a(n)=∑_{k=1}^{n} binom(n-1,k-1)*c(k)*a(n-k) where c(1)=1
 *            and c(k)=2^{k-1}-1 for k≥2
 * Range:     u8 n <= 5, u16 n <= 8, u32 n <= 13, u64 n <= 22, u128 n <= 37,
 *            big any n
 * Helpers:   ensure_capacity
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdlib.h>
#include "../utils/ensure_capacity.h"

/* dynamic arrays for memoisation */
static seq_t *dp      = NULL;  /* a values */
static size_t dp_len  = 0;     /* number of computed a values (dp[0]..dp[dp_len-1]) */
static size_t dp_cap  = 0;     /* allocated capacity of dp */

static seq_t *pow2    = NULL;  /* 2^i values */
static size_t pow2_len= 0;     /* number of computed powers (pow2[0]..pow2[pow2_len-1]) */
static size_t pow2_cap= 0;     /* allocated capacity of pow2 */

/* ------------------------------------------------------------------ */
/*  seq_t a(idx_t n)  –  OEIS sequence A307389                     */
/* ------------------------------------------------------------------ */
seq_t a(idx_t n)
{
    /* ---------- initialise dp array ---------- */
    if (dp_len == 0) {
        ensure_capacity(1, &dp_cap, (void **)&dp, sizeof(seq_t));
        dp[0] = (seq_t)1;
        dp_len = 1;
    }

    /* ---------- initialise pow2 array ---------- */
    if (pow2_len == 0) {
        ensure_capacity(1, &pow2_cap, (void **)&pow2, sizeof(seq_t));
        pow2[0] = (seq_t)1;   /* 2^0 */
        pow2_len = 1;
    }

    /* ---------- grow arrays to cover n ---------- */
    size_t needed = (size_t)n + 1;
    if (dp_cap < needed)
        ensure_capacity(needed, &dp_cap, (void **)&dp, sizeof(seq_t));
    if (pow2_cap < needed)
        ensure_capacity(needed, &pow2_cap, (void **)&pow2, sizeof(seq_t));

    /* ---------- extend pow2 table if required ---------- */
    if (pow2_len <= (size_t)n) {
        for (size_t i = pow2_len; i <= (size_t)n; ++i)
            pow2[i] = pow2[i-1] * (seq_t)2;
        pow2_len = (size_t)n + 1;
    }

    /* ---------- compute a values up to n ---------- */
    for (size_t i = dp_len; i <= (size_t)n; ++i) {
        seq_t sum = (seq_t)0;
        seq_t binom = (seq_t)1;                 /* binom(i-1,0) */

        for (size_t k = 1; k <= i; ++k) {
            seq_t ck;
            if (k == 1)
                ck = (seq_t)1;
            else {
                ck = pow2[k-1];        /* 2^{k-1} */
                ck = ck - (seq_t)1;    /* minus 1 */
            }

            seq_t term = binom * ck;
            term = term * dp[i - k];
            sum = sum + term;

            if (k < i) {
                seq_t tmp = binom * (seq_t)(i - k);
                binom = tmp / (seq_t)k;
            }
        }
        dp[i] = sum;
    }
    dp_len = (size_t)n + 1;

    return dp[(size_t)n];
}
