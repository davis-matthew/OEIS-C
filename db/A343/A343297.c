/*
 * A343297  Numbers k such that there are exactly two multisets of cardinality
 *          k where the sum equals the product (A033178(k)=2).
 * https://oeis.org/A343297
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 30, u16 n <= 49, u32 n <= 49, u64 n <= 49, u128 n <= 49,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* OEIS A343297 – numbers k such that exactly two multisets of size k
 * satisfy sum = product.  The function a(n) returns the nth term
 * (1‑based).  The implementation enumerates all non‑decreasing
 * tuples of integers ≥ 2, determines the corresponding k from
 * k = product – sum + m (where m = tuple length), and counts how many
 * tuples generate each k.  It returns the k’s for which the count is
 * exactly 2.  The implementation uses a static pre‑computation
 * up to a preset maximum; if more terms are required it doubles the
 * limit and recomputes.
 */

#include <stddef.h>
#include <stdint.h>
#include <stdbool.h>
#include <stdlib.h>

/* Configuration: pre‑compute up to this many k’s initially. */
#define INIT_KMAX 400000UL      /* enough to cover many terms (≈100 M of work) */

/* static data structures */
static idx_t    g_kmax     = 0;          /* current maximum k processed       */
static uint16_t *g_counts  = NULL;       /* number of tuples for each k      */
static seq_t   *g_terms   = NULL;       /* list of terms (k where count==2) */
static idx_t    g_termcnt = 0;           /* number of terms stored           */

/* ------------------------------------------------------------------ */
/* Recursive enumeration of tuples (non‑decreasing, factors ≥ 2).     */
static void dfs(idx_t product, idx_t sum, unsigned m,
                idx_t start, idx_t limit,
                uint16_t *counts, idx_t KMAX)
{
    idx_t max_f = limit / product;
    for (idx_t f = start; f <= max_f; ++f) {
        idx_t np = product * f;
        idx_t ns = sum + f;
        unsigned nm = m + 1;
        idx_t k = np - ns + nm;
        if (k <= KMAX && k >= nm) {
            if (counts[(idx_t)k] < 65535u) counts[(idx_t)k]++;   /* avoid overflow */
        }
        dfs(np, ns, nm, f, limit, counts, KMAX);
    }
}

/* ------------------------------------------------------------------ */
/* Initialise (or re‑initialise) the data structures. */
static void init_sequence(void)
{
    if (g_kmax) return;  /* already initialised */

    /* Allocate space for counts. */
    g_kmax     = INIT_KMAX;
    g_counts   = (uint16_t*)calloc(g_kmax + 1, sizeof(uint16_t));
    if (!g_counts) return;   /* out of memory – unlikely in tests */

    /* Enumerate all tuples up to 2*limit. */
    dfs(1, 0, 0, 2, 2 * g_kmax, g_counts, g_kmax);

    /* Build the terms array. */
    g_termcnt = 0;
    g_terms = (seq_t*)malloc(g_kmax * sizeof(seq_t));
    if (!g_terms) return;

    for (idx_t k = 1; k <= g_kmax; ++k) {
        if (g_counts[k] == 2) {
            g_terms[g_termcnt++] = (seq_t)k;
        }
    }
}

/* ------------------------------------------------------------------ */
/* Public function required by the build.   */
seq_t a(idx_t n)
{
    init_sequence();
    if (n == 0) return 0;                           /* safety */

    /* If the requested term is already computed, return it. */
    if (n <= g_termcnt) return g_terms[n - 1];

    /* If not enough terms, grow the limit and recompute entirely. */
    while (n > g_termcnt) {
        idx_t new_kmax = g_kmax * 2;

        uint16_t *new_counts = (uint16_t*)calloc(new_kmax + 1, sizeof(uint16_t));
        if (!new_counts) break;   /* allocation failed – give up */

        dfs(1, 0, 0, 2, 2 * new_kmax, new_counts, new_kmax);

        seq_t *new_terms = (seq_t*)malloc(new_kmax * sizeof(seq_t));
        if (!new_terms) { free(new_counts); break; }

        idx_t new_termcnt = 0;
        for (idx_t k = 1; k <= new_kmax; ++k) {
            if (new_counts[k] == 2) new_terms[new_termcnt++] = (seq_t)k;
        }

        /* Replace old data. */
        free(g_counts);
        free(g_terms);
        g_counts   = new_counts;
        g_terms    = new_terms;
        g_kmax     = new_kmax;
        g_termcnt  = new_termcnt;
    }

    /* After expansion (if any) the term should exist. */
    if (n <= g_termcnt) return g_terms[n - 1];
    return 0;   /* fallback – should not happen for requested n */
}
