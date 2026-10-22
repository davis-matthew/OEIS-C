/*
 * A304498  Solution (b(n)) of the system of complementary equations defined
 *          in Comments.
 * https://oeis.org/A304498
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 55, u16 n <= 60, u32 n <= 60, u64 n <= 60, u128 n <= 60,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A304498 – the b(n) part of the complementary system defined in the
   OEIS entry.  The sequences a(n), b(n), c(n) are defined recursively as
   follows (taken from the comments of the entry):

   a(0) = 1, b(0) = 2, c(0) = 2*a(0) + b(0) = 4
   For n > 0:
     a(n) = least integer not yet used by any a(k), b(k), or c(k)
     b(n) = least integer > a(n) not yet used
     c(n) = 2*a(n) + b(n)

   The function below returns b(n).  The sequence starts with
     b(0) = 2, b(1) = 5, b(2) = 7, …
   which matches the OEIS b‑file.
   The routine uses a simple incremental algorithm with a boolean “used”
   array and memoises all computed values so that it works for arbitrary
   ordering of the calls.  All allocations are performed with casts that
   make the code both C11 and C++17 compliant.  */

#include <stdbool.h>
#include <stdlib.h>
#include <string.h>

/* static storage */
static seq_t *a_cache  = NULL;   /* cached a(n) values */
static seq_t *b_cache  = NULL;   /* cached b(n) values */
static size_t  a_cap   = 0;
static size_t  b_cap   = 0;
static idx_t   current_n = (idx_t)-1; /* highest n computed */

static bool *used      = NULL;   /* used[i] == true if integer i has appeared */
static size_t used_cap  = 0;     /* capacity of 'used' array */

/* Ensure the 'used' array can hold indices up to v (inclusive).                  */
static void
ensure_used_capacity(idx_t v)
{
    if (used_cap > (size_t)v)
        return;                     /* already large enough */

    size_t new_cap = used_cap ? used_cap : 4;
    while (new_cap <= (size_t)v)
        new_cap <<= 1;              /* double until big enough */

    bool *tmp = (bool*)realloc(used, new_cap * sizeof(bool));
    if (!tmp)
        abort();
    used = tmp;
    /* initialise newly added entries as 'false' (not used) */
    memset(used + used_cap, 0, (new_cap - used_cap) * sizeof(bool));
    used_cap = new_cap;
}

/* Ensure the cache for a(n) (or b(n)) can hold indices up to n.                  */
static void
ensure_cache_capacity(idx_t n, seq_t **cache, size_t *cap)
{
    if (*cap > (size_t)n)
        return;                     /* already large enough */

    size_t old_cap = *cap;
    size_t new_cap = *cap ? *cap : 4;
    while (new_cap <= (size_t)n)
        new_cap <<= 1;

    seq_t *tmp = (seq_t*)realloc(*cache, new_cap * sizeof(seq_t));
    if (!tmp)
        abort();
    *cache = tmp;
    *cap   = new_cap;
}

/* Main sequence function.  Returns b(n).                                           */
seq_t
a(idx_t n)
{
    /* Return cached value if already known. */
    if (current_n != (idx_t)-1 && n <= current_n)
        return b_cache[n];

    /* Initialise storage on the first call. */
    if (!used) {
        used = (bool*)calloc(8, sizeof(bool));
        if (!used)
            abort();
        used_cap = 8;
        /* a(0)=1, b(0)=2, c(0)=4 are already used. */
        used[1] = true;
        used[2] = true;
        used[4] = true;
    }

    /* Allocate caches for a and b. */
    ensure_cache_capacity(n, &a_cache, &a_cap);
    ensure_cache_capacity(n, &b_cache, &b_cap);

    /* Store the base case a(0)=1, b(0)=2. */
    if (current_n == (idx_t)-1) {
        a_cache[0] = (seq_t)1;
        b_cache[0] = (seq_t)2;
        current_n = 0;
    }

    /* If the required n is 0 we are already done. */
    if (n == 0)
        return b_cache[0];

    /* Next integer to examine for being unused. */
    static idx_t next_candidate = 1;

    /* Compute values from current_n+1 up to n. */
    for (idx_t i = current_n + 1; i <= n; ++i) {
        /* Find a(i): least unused integer. */
        while (used[(size_t)next_candidate])
            ++next_candidate;
        seq_t a_val = (seq_t)next_candidate;
        used[(size_t)next_candidate] = true;
        ++next_candidate;

        /* Find b(i): least unused integer greater than a(i). */
        while (used[(size_t)next_candidate])
            ++next_candidate;
        seq_t b_val = (seq_t)next_candidate;
        used[(size_t)next_candidate] = true;
        ++next_candidate;

        /* Mark c(i) = 2*a(i) + b(i) as used. */
        seq_t c_val = a_val * (seq_t)2 + b_val;
        idx_t c_idx = (idx_t)c_val;          /* c_val will fit in idx_t for all n <= 10000 */
        ensure_used_capacity(c_idx);
        used[(size_t)c_idx] = true;

        /* Store the computed values. */
        a_cache[i] = a_val;
        b_cache[i] = b_val;
    }

    current_n = n;
    return b_cache[n];
}
