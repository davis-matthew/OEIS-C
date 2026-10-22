/*
 * A335390  a(n) = Sum_{k=0..n} Stirling2(n,k) * 2^binomial(k,2).
 * https://oeis.org/A335390
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 4, u16 n <= 6, u32 n <= 8, u64 n <= 11, u128 n <= 16,
 *            big any n
 * Helpers:   pow2
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* 
 * Formula: a(n) = Σ_{k=0..n} Stirling2(n,k) * 2^{k*(k-1)/2}
 * (Stirling2 is the Stirling number of the second kind.  The sequence
 * is the Stirling transform of 2^{k choose 2}.)
 */

#include <stddef.h>
#include <stdlib.h>
#include "../utils/pow2.h"

/* --------------------------------------------------------------------- */
/* Utility: grow a seq_t array to hold indices 0..need (inclusive)        */
/* --------------------------------------------------------------------- */
static void
ensure_capacity(size_t need, seq_t **p, size_t *cap)
{
    if (need + 1 > *cap) {
        size_t new_cap = (*cap == 0) ? 16 : (*cap << 1);
        while (new_cap <= need) new_cap <<= 1;
        *p = (seq_t *)realloc(*p, new_cap * sizeof(seq_t));
        *cap = new_cap;
    }
}

/* --------------------------------------------------------------------- */
/* Persistent storage for the sequence and auxiliary arrays              */
/* --------------------------------------------------------------------- */
static seq_t  *a_vals  = NULL;   /* a(n) values                              */
static size_t  a_cap   = 0;
static idx_t   a_done  = 0;       /* highest n already produced (0 ⇒ not yet) */

static seq_t  *s_arr   = NULL;   /* previous Stirling row S(n-1,k)          */
static size_t  s_cap   = 0;

static seq_t  *ns_arr  = NULL;   /* current Stirling row S(n,k)             */
static size_t  ns_cap  = 0;

static seq_t  *g_arr   = NULL;   /* g_arr[k] = 2^{k*(k-1)/2}                */
static size_t  g_cap   = 0;
static idx_t   g_done  = 0;       /* largest k whose g_arr entry is valid    */

/* --------------------------------------------------------------------- */
/* Public function: return a(n)                                          */
/* --------------------------------------------------------------------- */
seq_t a(idx_t n)
{
    /* Grow all needed arrays */
    ensure_capacity((size_t)n, &a_vals,  &a_cap);
    ensure_capacity((size_t)n, &s_arr,    &s_cap);
    ensure_capacity((size_t)n, &ns_arr,   &ns_cap);
    ensure_capacity((size_t)n, &g_arr,    &g_cap);

    /* Initialise base case once */
    if (a_done == 0) {
        a_vals[0] = (seq_t)1;        /* a(0) = 1                         */
        s_arr[0]  = (seq_t)1;        /* S(0,0) = 1                       */
    }

    /* Pre‑compute 2^{k*(k-1)/2} for all k up to n if not already done */
    if (n > g_done) {
        for (idx_t k = g_done + 1; k <= n; ++k) {
            idx_t exp = (k * (k - 1)) / 2;
            g_arr[k]  = pow2(exp);
        }
        g_done = n;
    }

    /* If we already have the requested value, return it */
    if (a_done >= n)
        return a_vals[n];

    /* Compute a(m) for all m = a_done+1 .. n */
    for (idx_t m = a_done + 1; m <= n; ++m) {
        ns_arr[0] = (seq_t)0;            /* S(m,0) = 0                       */

        /* Compute Stirling numbers of the second kind for row m */
        for (idx_t k = 1; k <= m; ++k) {
            seq_t val = s_arr[k - 1];     /* S(m-1,k-1)                     */
            if (k < m)                   /* add k * S(m-1,k) if it exists  */
                val = val + (seq_t)k * s_arr[k];
            ns_arr[k] = val;
        }

        /* Accumulate a(m) = Σ_{k=1..m} S(m,k) * g_arr[k] */
        seq_t sum = (seq_t)0;
        for (idx_t k = 1; k <= m; ++k)
            sum = sum + ns_arr[k] * g_arr[k];
        a_vals[m] = sum;

        /* Prepare for next iteration: swap the row buffers */
        seq_t *tmp = s_arr;
        s_arr    = ns_arr;
        ns_arr   = tmp;
    }

    a_done = n;
    return a_vals[n];
}
