/*
 * A323756  a(1) = 2; for n >= 2, if a(n-1) has not yet been assigned, then
 *          a(n-1) = 1 and a(2*n-1) = 2, otherwise a(2*n) = 3.
 * https://oeis.org/A323756
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 210, u16 n <= 210, u32 n <= 210, u64 n <= 210,
 *            u128 n <= 210, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A323756
   a(1) = 2;
   for n ≥ 2:
        if a(n-1) has not yet been assigned
                a(n-1) = 1   and   a(2*n-1) = 2
        else
                a(2*n)   = 3
   The implementation keeps a dynamic cache and greedily fills all
   required values up to the highest requested index.  It is correct
   for calls in arbitrary order and runs in linear total time. */

#include <stdlib.h>
#include <stdbool.h>

/* Helper for realloc that works in both C and C++ */
static void* realloc_cast(void* ptr, size_t size)
{
    return realloc(ptr, size);
}

/* Global cache state */
static seq_t  *values     = NULL; /* cached terms                       */
static bool   *assigned   = NULL; /* flag for each index                */
static size_t  capacity   = 0;    /* current capacity of both arrays    */
static idx_t   step_done  = 0;    /* largest step (n) processed         */

/* Ensure arrays have room for indices up to need (inclusive) */
static void ensure_capacity(idx_t need)
{
    if (need < capacity)
        return;

    size_t new_cap = capacity ? capacity * 2 : 64;
    while (new_cap <= (size_t)need)
        new_cap <<= 1;

    seq_t *tmp_v = (seq_t*)realloc_cast(values, new_cap * sizeof(seq_t));
    if (!tmp_v)
        abort();
    values = tmp_v;

    bool *tmp_a = (bool*)realloc_cast(assigned, new_cap * sizeof(bool));
    if (!tmp_a)
        abort();
    for (size_t i = capacity; i < new_cap; ++i)
        tmp_a[i] = false;
    assigned = tmp_a;
    capacity = new_cap;
}

/* Entry point required by the test harness */
seq_t a(idx_t n)
{
    if (n == 0)
        return (seq_t)0; /* undefined for n=0, but return 0 safely */

    /* Lazy initialization on first call */
    if (step_done == 0)
    {
        ensure_capacity(2);          /* indices 0 … 2 needed */
        values[1] = (seq_t)2;        /* a(1) = 2                */
        assigned[1] = true;
        step_done = 1;               /* step 1 considered done */
    }

    /* We must process steps up to n+1 to guarantee that a(n) is assigned.
       (a(k) is assigned when step k+1 is executed.) */
    idx_t target_step = n + 1;
    for (idx_t step = step_done + 1; step <= target_step; ++step)
    {
        idx_t prev = step - 1;
        if (!assigned[prev])        /* a(prev) unassigned                */
        {
            /* a(prev) = 1 */
            ensure_capacity(prev);
            values[prev] = (seq_t)1;
            assigned[prev] = true;

            /* a(2*step-1) = 2 */
            idx_t idx = 2 * step - 1;
            ensure_capacity(idx);
            values[idx] = (seq_t)2;
            assigned[idx] = true;
        }
        else                        /* a(prev) already assigned           */
        {
            /* a(2*step) = 3 */
            idx_t idx = 2 * step;
            ensure_capacity(idx);
            values[idx] = (seq_t)3;
            assigned[idx] = true;
        }
    }
    step_done = target_step;

    /* After the loop a(n) must be assigned; return it. */
    return values[n];
}
