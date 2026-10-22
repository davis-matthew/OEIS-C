/*
 * A375990  Expansion of e.g.f. (1 + 2 * log(1 - x))^2.
 * https://oeis.org/A375990
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign, easy
 * Formula:   a(n) = -4*(n-1)! + 8*|Stirling1(n,2)| (n ≥ 0)
 * Range:     i8 n <= 4, i16 n <= 7, i32 n <= 12, i64 n <= 20, i128 n <= 21,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>

/* --------- dynamic arrays for factorials and |Stirling1(n,2)| ------- */

static seq_t *fact   = NULL;   /* fact[i] = i!                */
static size_t fact_cap = 0;
static idx_t   fact_max = 0;   /* largest i with fact[i] computed */

static seq_t *u2     = NULL;   /* u2[n] = |Stirling1(n,2)|   */
static size_t u2_cap = 0;
static idx_t   u2_max = 0;     /* largest n with u2[n] computed */

/* enlarge an array to hold at least need elements */
static void ensure_capacity(void **arr, size_t *cap, size_t need) {
    size_t new_cap = *cap;
    while (new_cap < need) {
        new_cap = (new_cap == 0) ? 8 : new_cap * 2;
    }
    if (new_cap != *cap) {
        *arr = realloc(*arr, new_cap * sizeof(seq_t));
        *cap = new_cap;
    }
}

/* --------------------------------------------------------------- */
/*  factorials  --------------------------------------------------- */
/* --------------------------------------------------------------- */
static void ensure_fact(idx_t n) {
    if (n <= fact_max) return;
    ensure_capacity((void **)&fact, &fact_cap, (size_t)(n + 1));
    if (fact_max == 0) {                 /* init fact[0] = 0! = 1 */
        fact[0] = (seq_t)1;
        fact_max = 0;
    }
    for (idx_t i = fact_max + 1; i <= n; ++i) {
        fact[i] = fact[i - 1] * (seq_t)i; /* factorial recurrence */
    }
    fact_max = n;
}

/* --------------------------------------------------------------- */
/*  |Stirling1(n,2)|  -------------------------------------------- */
/* --------------------------------------------------------------- */
static void ensure_u2(idx_t n) {
    if (n < 2) return;                    /* values start at n=2 */
    if (n <= u2_max) return;
    ensure_capacity((void **)&u2, &u2_cap, (size_t)(n + 1));
    if (u2_max < 2) {                     /* initialise base for n=2 */
        u2[2] = (seq_t)1;
        u2_max = 2;
    }
    ensure_fact(n - 1);                    /* need (i-2)! for i up to n */

    for (idx_t i = u2_max + 1; i <= n; ++i) {
        if (i == 2) {
            u2[i] = (seq_t)1;
        } else {
            /* |Stirling1(i,2)| = (i-2)! + (i-1)*|Stirling1(i-1,2)| */
            u2[i] = fact[i - 2] + ((seq_t)(i - 1)) * u2[i - 1];
        }
    }
    u2_max = n;
}

/* --------------------------------------------------------------- */
/*  sequence function  -------------------------------------------- */
/* --------------------------------------------------------------- */
seq_t a(idx_t n) {
    if (n == 0) return (seq_t)1;           /* a(0) = 1 by definition */
    if (n == 1) return (seq_t)(-4);        /* a(1) = -4 */

    /* compute the two required components */
    ensure_fact(n - 1);                    /* factorial up to (n-1)!   */
    ensure_u2(n);                          /* |Stirling1(n,2)|        */

    seq_t term1 = -4 * fact[n - 1];        /* -4*(n-1)!                */
    seq_t term2 = 8 * u2[n];               /* 8*|Stirling1(n,2)|      */
    return term1 + term2;
}
