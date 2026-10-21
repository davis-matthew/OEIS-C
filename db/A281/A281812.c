/*
 * A281812  Expansion of Sum_{i>=1} mu(i)^2*x^i / (1 - Sum_{j>=1}
 *          mu(j)^2*x^j)^2, where mu() is the Moebius function (A008683).
 * https://oeis.org/A281812
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 7, u16 n <= 14, u32 n <= 30, u64 n <= 36, u128 n <= 36,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Total number of parts in all compositions of n into square–free parts
   (OEIS A281812).  Computed by dynamic programming: let f[n] be the
   number of compositions of n into square‑free parts, and g[n] the
   total number of parts in all those compositions.  Then

      f[0] = 1,  g[0] = 0
      f[n] = Σ_{s∈S, s≤n} f[n‑s]
      g[n] = Σ_{s∈S, s≤n} (g[n‑s] + f[n‑s]),

   where S is the set of square‑free integers ≤ n.  The arrays for f, g
   and the list of square‑free numbers are grown lazily as larger n
   are requested.  The function a(idx_t n) returns g[n] for the
   requested n (n≥1).  All arithmetic is performed in seq_t, the
   integer type supplied by the build environment. */

#include <stddef.h>
#include <stdbool.h>
#include <stdlib.h>

/* --------------------------------------------------------------------- */
/*  Static state used for memoisation                                    */
/* --------------------------------------------------------------------- */
static idx_t      g_max_prepped   = 0;      /* largest n processed so far */
static seq_t     *g_f             = NULL;  /* f[n] */
static seq_t     *g_g             = NULL;  /* g[n] */
static bool      *g_sqfree        = NULL;  /* array of square‑free flags */
static idx_t     *g_sqfree_list   = NULL;  /* list of square‑free numbers */
static size_t    g_sqfree_count   = 0;


/* --------------------------------------------------------------------- */
/*  Prepare tables up to new_n (new_n > current size)                    */
/* --------------------------------------------------------------------- */
static void g_prepare_up_to(idx_t new_n)
{
    size_t new_size = (size_t)(new_n + 1);

    /* 1. Reallocate f and g arrays */
    g_f = (seq_t *)realloc(g_f, new_size * sizeof(seq_t));
    g_g = (seq_t *)realloc(g_g, new_size * sizeof(seq_t));

    /* 2. Recompute square‑free boolean array */
    g_sqfree = (bool *)realloc(g_sqfree, new_size * sizeof(bool));
    for (size_t i = 0; i < new_size; ++i) g_sqfree[i] = true;
    g_sqfree[0] = false;               /* 0 is not considered */

    for (idx_t p = 2; p * p <= new_n; ++p) {
        idx_t p2 = p * p;
        for (idx_t m = p2; m <= new_n; m += p2) g_sqfree[m] = false;
    }

    /* 3. Build list of square‑free numbers */
    size_t count = 0;
    for (idx_t i = 1; i <= new_n; ++i)
        if (g_sqfree[i]) ++count;

    g_sqfree_list = (idx_t *)realloc(g_sqfree_list, count * sizeof(idx_t));
    size_t k = 0;
    for (idx_t i = 1; i <= new_n; ++i)
        if (g_sqfree[i]) g_sqfree_list[k++] = i;

    g_sqfree_count = count;

    /* 4. Initialise base cases if first time */
    if (g_max_prepped == 0) {
        g_f[0] = 1;
        g_g[0] = 0;
    }

    /* 5. Dynamic programming for new values */
    for (idx_t n = g_max_prepped + 1; n <= new_n; ++n) {
        seq_t sum_f = 0;
        seq_t sum_g = 0;

        for (size_t j = 0; j < g_sqfree_count && g_sqfree_list[j] <= n; ++j) {
            idx_t s   = g_sqfree_list[j];
            idx_t rem = n - s;
            sum_f += g_f[rem];
            sum_g += g_g[rem] + g_f[rem];
        }

        g_f[n] = sum_f;
        g_g[n] = sum_g;
    }

    g_max_prepped = new_n;
}


/* --------------------------------------------------------------------- */
/*  Ensure that tables have been prepared up to n                        */
/* --------------------------------------------------------------------- */
static void g_ensure_prepared(idx_t n)
{
    if (n <= g_max_prepped) return;
    g_prepare_up_to(n);
}


/* --------------------------------------------------------------------- */
/*  Public API                                                            */
/* --------------------------------------------------------------------- */
seq_t a(idx_t n)
{
    if (n == 0) {          /* offset is 1, but return 0 for safety */
        seq_t zero = 0;
        return zero;
    }
    g_ensure_prepared(n);
    return g_g[n];
}
