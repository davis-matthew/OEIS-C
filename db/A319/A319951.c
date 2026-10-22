/*
 * A319951  Take Golomb's sequence A001462 and extend all the runs by 1;
 *          prepend an initial 0.
 * https://oeis.org/A319951
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 102, u16 n <= 102, u32 n <= 102, u64 n <= 102,
 *            u128 n <= 102, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  Formula (OEIS):
     a(0) = 0                     /* initial value
     a(1) = 1;  a(2) = 1          /* first two values
     a(n) = A001462( n - A001462( n - A001462(n) ) )   for n ≥ 3
   (the sequence is the Golomb sequence with each run extended by one,
   preceded by a leading zero).  */

#include <stdlib.h>

static seq_t *a_cache     = NULL;   /* cached values of a(n)           */
static size_t a_capacity  = 0;      /* allocated capacity of a_cache   */
static idx_t  a_len       = 0;      /* number of values stored         */

static seq_t *g_cache     = NULL;   /* cached values of Golomb g(k)    */
static size_t g_capacity  = 0;      /* allocated capacity of g_cache   */
static idx_t  next_k      = 1;      /* next integer k to process      */

/*  Ensure that a_cache contains values up to index n (0‑based). */
static void
ensure_a_upto(idx_t n)
{
    /*  Initialise caches on the first call  */
    if (a_cache == NULL) {
        a_capacity = 64;
        a_cache = (seq_t *)malloc(a_capacity * sizeof(seq_t));
        a_cache[0] = 0;       /* a(0) */
        a_len = 1;
    }

    if (g_cache == NULL) {
        g_capacity = 64;
        g_cache = (seq_t *)malloc(g_capacity * sizeof(seq_t));
        g_cache[0] = 0;
        g_cache[1] = 1;       /* first Golomb value g(1) */
    }

    /*  Append new segments until we have enough entries  */
    while (a_len <= n) {
        idx_t k = next_k;

        /*  Grow Golomb array if necessary  */
        if ((size_t)k >= g_capacity) {
            size_t new_cap = g_capacity * 2;
            seq_t *tmp = (seq_t *)realloc(g_cache, new_cap * sizeof(seq_t));
            g_cache = tmp;
            g_capacity = new_cap;
        }

        /*  Compute g(k) – Golomb's sequence  */
        if (k == 1) {
            g_cache[1] = (seq_t)1;
        } else {
            /*  g(k) = 1 + g( k - g( g(k-1) ) )  */
            idx_t prev = (idx_t)g_cache[k - 1];
            idx_t idx   = k - (idx_t)g_cache[prev];
            g_cache[k] = 1 + g_cache[idx];
        }

        /*  Number of times k appears in extended run = g(k)+1  */
        seq_t run_len = g_cache[k] + 1;

        /*  Grow value cache if needed  */
        while (a_len + run_len > a_capacity) {
            size_t new_cap = a_capacity * 2;
            seq_t *tmp = (seq_t *)realloc(a_cache, new_cap * sizeof(seq_t));
            a_cache = tmp;
            a_capacity = new_cap;
        }

        /*  Append the run to a_cache  */
        for (seq_t i = 0; i < run_len; ++i) {
            a_cache[a_len++] = (seq_t)k;
        }

        /*  Prepare for next integer  */
        ++next_k;
    }
}

seq_t
a(idx_t n)
{
    ensure_a_upto(n);
    return a_cache[n];
}
