/*
 * A377429  Expansion of e.g.f. (1/x) * Series_Reversion( x*(1 + log(1-x))^4
 *          ).
 * https://oeis.org/A377429
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 2, u16 n <= 4, u32 n <= 6, u64 n <= 11, u128 n <= 16,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A377429:
   a(0) = 1
   a(n) = 4 * Σ_{k=1..n} S1(n,k) * Π_{t=4n+5}^{4n+k+3} t   (n ≥ 1)
   where S1(n,k) is the unsigned Stirling number of the first kind. */

/* Note: The code uses only integer operations compatible with both C11
   and C++17.  It relies on the standard allocation functions
   malloc()/realloc() and the minimal set of standard headers. */

#include <stdlib.h>

/* Cached value table: a_cache[i] holds a(i).  The capacity is
   increased as needed. */
static seq_t *a_cache = NULL;   /* values of the sequence */
static size_t  a_cap   = 0;     /* allocated capacity */

/* Current Stirling row.  stirling[k] = S1(current_n, k)  (0 ≤ k ≤ current_n). */
static seq_t *stirling = NULL;   /* current Stirling row */
static size_t  stir_cap = 0;     /* allocated capacity of stirling array */

/* Size of the current Stirling row (current_n + 1). */
static size_t stir_size = 0;

/* Highest n for which the sequence has been computed. */
static size_t max_cache = 0;

seq_t a(idx_t n)
{
    /* ------------------------------------------------------------------------
       Initialise storage on first call.
       ------------------------------------------------------------------------ */
    if (a_cache == NULL) {
        /* Start with capacity for n = 0 only. */
        a_cap = 1;
        a_cache = (seq_t *)malloc(a_cap * sizeof(seq_t));
        a_cache[0] = (seq_t)1;         /* a(0) = 1 */

        /* Stirling row for n = 0: S1(0,0) = 1. */
        stir_cap   = 1;
        stirling   = (seq_t *)malloc(stir_cap * sizeof(seq_t));
        stirling[0] = (seq_t)1;          /* S1(0,0) = 1 */
        stir_size   = 1;

        max_cache   = 0;
    }

    /* If the requested n is already cached, simply return it. */
    if (n <= max_cache) {
        return a_cache[n];
    }

    /* ------------------------------------------------------------------------
       Grow the a_cache array to hold all values up to n.
       ------------------------------------------------------------------------ */
    if (n >= a_cap) {
        size_t new_cap = a_cap * 2;
        if (new_cap <= (size_t)n) new_cap = (size_t)n + 1;
        seq_t *tmp = (seq_t *)realloc(a_cache, new_cap * sizeof(seq_t));
        if (tmp == NULL) { return (seq_t)0; } /* allocation failure – unlikely */
        a_cache = tmp;
        a_cap   = new_cap;
    }

    /* ------------------------------------------------------------------------
       Grow the Stirling array to be able to store a row of size (n+1).
       ------------------------------------------------------------------------ */
    if (n + 1 > stir_cap) {
        size_t new_cap = stir_cap * 2;
        if (new_cap <= (size_t)(n + 1)) new_cap = (size_t)(n + 1);
        seq_t *tmp = (seq_t *)realloc(stirling, new_cap * sizeof(seq_t));
        if (tmp == NULL) { return (seq_t)0; } /* allocation failure – unlikely */
        stirling = tmp;
        stir_cap = new_cap;
    }

    /* ------------------------------------------------------------------------
       Compute successive terms from (max_cache + 1) up to n.
       ------------------------------------------------------------------------ */
    for (idx_t m = (idx_t)(max_cache + 1); m <= n; ++m) {
        /* --------------------------------------------------------------------
           Update Stirling row from size m to m+1.
           The previous row has length m (indices 0 .. m-1).
           -------------------------------------------------------------------- */
        /* The element at index m is currently uninitialized.  Set it to
           the previous top value S1(m-1, m-1). */
        stirling[m] = stirling[m - 1];

        /* Update the interior entries from high to low to avoid using
           values that have already been updated. */
        for (idx_t k = m - 1; k > 0; --k) {
            stirling[k] = stirling[k - 1] + (seq_t)(m - 1) * stirling[k];
        }

        /* S1(m,0) = 0 for m ≥ 1. */
        stirling[0] = (seq_t)0;

        /* Update the size of the current Stirling row. */
        stir_size = (size_t)(m + 1);

        /* --------------------------------------------------------------------
           Compute a(m) using the product formula derived from the EGF
           definition.  For m ≥ 1 we sum from k = 1 to m.
           -------------------------------------------------------------------- */
        if (m == 0) {
            a_cache[m] = (seq_t)1;
            continue;
        }

        seq_t prod = (seq_t)1;   /* product_{t=4m+5}^{4m+k+3} t   */
        seq_t sum = (seq_t)0;

        for (idx_t k = 1; k <= m; ++k) {
            /* Add the current term. */
            sum += prod * stirling[k];

            /* Multiply prod by the next factor for the following k. */
            prod *= (seq_t)(4 * m + (seq_t)k + 4);
        }

        /* Multiply by 4 as per the formula. */
        a_cache[m] = sum * (seq_t)4;
    }

    /* Update the maximum cached index. */
    max_cache = n;

    return a_cache[n];
}
