/*
 * A323420  Lexicographically earliest sequence of positive integers such that
 *          for any n > 0, a(n + a(n)) > a(n).
 * https://oeis.org/A323420
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 10000, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A323420: Lexicographically earliest sequence of positive integers
   satisfying a(n + a(n)) > a(n) for all n > 0.  The sequence is constructed
   greedily:  for each new index n choose the smallest positive integer
   that satisfies all constraints imposed by earlier terms.  The implementation
   keeps a table `need[n]` which stores the minimal value that a[n] must have
   because some earlier term j with j + a(j) = n requires a[n] > a[j]. */

#include <stdlib.h>
#include <string.h>

/* ------------------------------------------------------------------ */
/* Static tables holding the sequence values and pending minima.      */
/* ------------------------------------------------------------------ */
static seq_t *seq   = NULL;   /* a(1), a(2), ...  – 1‑based indexing */
static seq_t *need  = NULL;   /* minimal required value for each index   */
static size_t  cap   = 0;     /* allocated capacity of the tables        */
static size_t  maxn  = 0;     /* largest n that has been processed       */

/* ------------------------------------------------------------------ */
/* Expand the tables so that indices up to `n` are valid.  They grow
   geometrically to keep amortised O(1) allocation per element.        */
/* ------------------------------------------------------------------ */
static void ensure_capacity(idx_t n)
{
    if ((size_t)n < cap)
        return;

    size_t new_cap = cap ? cap : 1;
    while (new_cap <= (size_t)n)
        new_cap <<= 1;

    seq  = (seq_t *)realloc(seq,  new_cap * sizeof(seq_t));
    need = (seq_t *)realloc(need, new_cap * sizeof(seq_t));

    /* Initialise freshly allocated slots to zero.  Zero means "no requirement". */
    memset(seq  + cap, 0, (new_cap - cap) * sizeof(seq_t));
    memset(need + cap, 0, (new_cap - cap) * sizeof(seq_t));

    cap = new_cap;
}

/* ------------------------------------------------------------------ */
/* Record that index `pos` must eventually contain a value greater
   than `val`.  `val` is the value just chosen for some earlier index.
   The requirement is stored as the maximum over all such constraints.  */
/* ------------------------------------------------------------------ */
static void update_need(idx_t pos, seq_t val)
{
    /* Ensure the target position exists. */
    if ((size_t)pos >= cap)
        ensure_capacity(pos);

    seq_t new_req = val + (seq_t)1;   /* a(pos) must be > val          */
    if (new_req > need[pos])
        need[pos] = new_req;
}

/* ------------------------------------------------------------------ */
/* Main sequence routine.  Calls are idempotent and may arrive in any
   order.  The function returns a(n) for n >= 1.                         */
/* ------------------------------------------------------------------ */
seq_t a(idx_t n)
{
    if (n == 0)
        return 0;    /* value undefined for n == 0 – never called */

    /* Make room for the requested index and any future constraints. */
    ensure_capacity(n);

    /* Compute all needed terms up to n that have not been processed yet. */
    for (idx_t i = maxn + 1; i <= n; ++i)
    {
        /* Determine the smallest allowed value for this position. */
        seq_t required = (need[i] == 0) ? (seq_t)1 : need[i];
        seq[i] = required;

        /* Propagate the constraint that the future position must be larger. */
        /* Compute target index i + a(i).  The cast to idx_t is safe because
           a(i) is guaranteed to fit in an index (otherwise the relation
           a(i + a(i)) would be undefined). */
        idx_t target = i + (idx_t)seq[i];
        if (target < cap || target > maxn)   /* update even if beyond current max */
            update_need(target, seq[i]);
    }

    maxn = (size_t)n;
    return seq[n];
}
