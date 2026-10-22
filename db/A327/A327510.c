/*
 * A327510  Number of set partitions of [n] where each subset is again
 *          partitioned into nine nonempty subsets.
 * https://oeis.org/A327510
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 10, u16 n <= 12, u32 n <= 16, u64 n <= 26, u128 n <= 40,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* OEIS A327510
   Recurrence used: a(0)=1, a(n)=0 for 1≤n<9; for n≥9
   a(n)=∑_{j=0}^{n-9} binom(n-1,j) · S(n-j,9) · a(j)
   where S(m,9) is the Stirling number of the second kind with 9 blocks. */

#include <stddef.h>
#include <stdlib.h>
#include <stdbool.h>

/* ------------------------------------------------------------------ */
/*  Static data for caching a(n)                                         */
static seq_t *a_cache   = NULL;   /* a[0 .. a_cap-1]           */
static size_t a_cap    = 0;       /* allocated size            */
static idx_t  a_max    = 0;       /* largest computed index    */

/* ------------------------------------------------------------------ */
/*  Static data for caching S(m,9)                                       */
static seq_t *S_cache   = NULL;   /* S[0 .. S_cap-1] = S(m,9)  */
static size_t S_cap    = 0;       /* allocated size            */
static idx_t  S_max    = 0;       /* largest computed index    */
static seq_t  S_row[10] = {0};    /* current row for Stirling   */

/* ------------------------------------------------------------------ */
/*  Ensure S_cache contains values up to S[n] = S(n,9)                  */
static void ensure_s_cache(idx_t n)
{
    if (S_cache == NULL) {
        S_cap = 1;
        S_cache = (seq_t *)malloc(sizeof(seq_t));
        if (!S_cache) abort();
        S_cache[0] = 0;
        S_max = 0;
        for (int t = 0; t <= 9; ++t) S_row[t] = (t == 0 ? 1 : 0);
    }
    if (n <= S_max) return;

    if ((size_t)(n + 1) > S_cap) {
        size_t newcap = n + 1;
        seq_t *tmp = (seq_t *)realloc(S_cache, newcap * sizeof(seq_t));
        if (!tmp) abort();
        memset(tmp + S_cap, 0, (newcap - S_cap) * sizeof(seq_t));
        S_cache = tmp;
        S_cap = newcap;
    }
    for (idx_t k = S_max + 1; k <= n; ++k) {
        seq_t newS[10];
        newS[0] = 0;
        for (int t = 1; t <= 9; ++t)
            newS[t] = (seq_t)t * S_row[t] + S_row[t - 1];
        for (int t = 0; t <= 9; ++t) S_row[t] = newS[t];
        S_cache[k] = S_row[9];
    }
    S_max = n;
}

/* ------------------------------------------------------------------ */
/*  Ensure a_cache contains values up to a[n]                          */
static void ensure_a_cache(idx_t n)
{
    if (a_cache == NULL) {
        a_cap = 1;
        a_cache = (seq_t *)malloc(sizeof(seq_t));
        if (!a_cache) abort();
        a_cache[0] = 1;      /* a(0) = 1 */
        a_max = 0;
    }
    if (n <= a_max) return;

    if ((size_t)(n + 1) > a_cap) {
        size_t newcap = n + 1;
        seq_t *tmp = (seq_t *)realloc(a_cache, newcap * sizeof(seq_t));
        if (!tmp) abort();
        memset(tmp + a_cap, 0, (newcap - a_cap) * sizeof(seq_t));
        a_cache = tmp;
        a_cap = newcap;
    }

    for (idx_t m = a_max + 1; m <= n; ++m) {
        seq_t value = 0;
        if (m >= 9) {
            seq_t comb = 1;                 /* binom(m-1,0) */
            idx_t maxj = m - 9;
            for (idx_t j = 0; j <= maxj; ++j) {
                seq_t term = comb;
                term *= S_cache[m - j];
                term *= a_cache[j];
                value += term;
                if (j < maxj) {
                    /* comb = binom(m-1, j+1) */
                    comb = (comb * (seq_t)(m - 1 - j)) / (seq_t)(j + 1);
                }
            }
        }
        a_cache[m] = value;
    }
    a_max = n;
}

/* ------------------------------------------------------------------ */
/*  Public function                                                      */
seq_t a(idx_t n)
{
    ensure_s_cache(n);
    ensure_a_cache(n);
    return a_cache[n];
}
