/*
 * A277962  Least k such that A006577(k) = A006577(n) + A006577(n+1).
 * https://oeis.org/A277962
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 17, u16 n <= 653, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computation of OEIS A277962
   a(n) = least k such that A006577(k) = A006577(n) + A006577(n+1)
   where A006577(m) is the number of Collatz (3x+1) steps to reach 1.
   The function builds tables of stopping times and first indices
   incrementally, so calls may come in any order.
*/

#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>

/* -----------------------------------------------------------------------
 * Helper: compute the Collatz stopping time for a given idx_t.
 * Uses a 128‑bit intermediate to avoid overflow for small n.
 * ----------------------------------------------------------------------- */
static seq_t collatz_steps(idx_t n)
{
    seq_t steps = 0;
    __int128 x = n;
    while (x > 1) {
        if ((x & 1) == 0)           /* even: x = x/2 */
            x >>= 1;
        else                        /* odd:  x = 3x+1 */
            x = 3 * x + 1;
        ++steps;
    }
    return steps;
}

/* -----------------------------------------------------------------------
 * Tables: step_vals[i]   = A006577(i)
 *         first_k_by_step[s] = first k with A006577(k)=s
 * ----------------------------------------------------------------------- */
static seq_t *step_vals          = NULL;  /* dynamic array */
static size_t step_vals_cap      = 0;
static idx_t  max_k_computed     = 0;      /* largest index computed */

static seq_t *first_k_by_step    = NULL;  /* dynamic array */
static size_t first_k_cap        = 0;

/* -----------------------------------------------------------------------
 * Ensure capacity of step_vals up to req (inclusive)
 * ----------------------------------------------------------------------- */
static void ensure_step_capacity(idx_t req)
{
    if ((size_t)req < step_vals_cap)
        return;
    size_t new_cap = step_vals_cap ? step_vals_cap * 2 : 1024;
    while (new_cap <= (size_t)req)
        new_cap *= 2;
    seq_t *tmp = (seq_t *)realloc(step_vals, new_cap * sizeof(seq_t));
    if (!tmp) exit(EXIT_FAILURE);
    /* initialise newly allocated slots to 0 */
    for (size_t i = step_vals_cap; i < new_cap; ++i)
        tmp[i] = 0;
    step_vals = tmp;
    step_vals_cap = new_cap;
}

/* -----------------------------------------------------------------------
 * Ensure capacity of first_k_by_step up to step (inclusive)
 * ----------------------------------------------------------------------- */
static void ensure_first_k_capacity(idx_t step)
{
    if ((size_t)step < first_k_cap)
        return;
    size_t new_cap = first_k_cap ? first_k_cap * 2 : 128;
    while (new_cap <= (size_t)step)
        new_cap *= 2;
    seq_t *tmp = (seq_t *)realloc(first_k_by_step, new_cap * sizeof(seq_t));
    if (!tmp) exit(EXIT_FAILURE);
    for (size_t i = first_k_cap; i < new_cap; ++i)
        tmp[i] = 0;
    first_k_by_step = tmp;
    first_k_cap = new_cap;
}

/* -----------------------------------------------------------------------
 * Compute stopping times up to and including k.
 * Updates max_k_computed and first_k_by_step accordingly.
 * ----------------------------------------------------------------------- */
static void compute_up_to(idx_t k)
{
    ensure_step_capacity(k);
    for (idx_t i = max_k_computed + 1; i <= k; ++i) {
        seq_t s  = collatz_steps(i);
        step_vals[i] = s;
        idx_t s_idx = (idx_t)s;
        ensure_first_k_capacity(s_idx);
        if (first_k_by_step[s_idx] == 0)        /* first occurrence */
            first_k_by_step[s_idx] = i;
    }
    max_k_computed = k;
}

/* -----------------------------------------------------------------------
 * Public function required by the build system
 * ----------------------------------------------------------------------- */
seq_t a(idx_t n)
{
    /* Lazy allocation of static tables */
    if (!step_vals) {
        step_vals_cap = 1024;
        step_vals = (seq_t *)calloc(step_vals_cap, sizeof(seq_t));
        if (!step_vals) exit(EXIT_FAILURE);
    }
    if (!first_k_by_step) {
        first_k_cap = 128;
        first_k_by_step = (seq_t *)calloc(first_k_cap, sizeof(seq_t));
        if (!first_k_by_step) exit(EXIT_FAILURE);
    }

    /* Ensure we know the stopping times for n and n+1 */
    if (max_k_computed < n + 1)
        compute_up_to(n + 1);

    /* Target stopping time: sum of the two consecutive values */
    idx_t target_idx = (idx_t)step_vals[n] + (idx_t)step_vals[n + 1];
    ensure_first_k_capacity(target_idx);

    /* If already found, return it */
    if (first_k_by_step[target_idx] != 0)
        return first_k_by_step[target_idx];

    /* Extend tables until the target stopping time appears */
    idx_t k = max_k_computed + 1;
    while (true) {
        compute_up_to(k);
        if (first_k_by_step[target_idx] != 0)
            return first_k_by_step[target_idx];
        ++k;
    }
}
