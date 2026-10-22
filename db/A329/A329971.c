/*
 * A329971  Expansion of 1 / (1 - 2 * Sum_{k>=1} x^(k^2)).
 * https://oeis.org/A329971
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(0) = 1, a(n) = 2 * Σ_{k=1..⌊√n⌋} a(n - k^2) for n ≥ 1
 * Range:     u8 n <= 7, u16 n <= 14, u32 n <= 28, u64 n <= 32, u128 n <= 32,
 *            big any n
 * Helpers:   alloc_seq_array
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>   /* size_t, NULL */
#include <stdlib.h>   /* malloc, free */
#include <string.h>   /* memset, memcpy */
#include "../utils/alloc_seq_array.h"   /* alloc_seq_array(size_t) */

/* Static memoization storage */
static seq_t *a_arr   = NULL;   /* array of computed terms */
static size_t cap     = 0;      /* allocated capacity (size in elements) */
static idx_t  a_max_n = 0;      /* highest index already computed (a(0) included) */

/* Ensure that a_arr has room for indices 0..n (inclusive). */
static void
ensure_capacity(idx_t n)
{
    size_t newcap = (size_t)(n + 1);
    if (newcap <= cap)
        return;          /* already big enough */

    /* Allocate new array via the provided helper (handles arbitrary‑precision types). */
    seq_t *new_a = alloc_seq_array(newcap);
    if (!new_a)
        return;          /* allocation failure – not expected in tests */

    /* Copy existing data to the new array. */
    if (cap > 0)
    {
        /* seq_t may be not trivially copyable; use memcpy on the raw block. */
        memcpy(new_a, a_arr, cap * sizeof(seq_t));
        free(a_arr);     /* free previous block – works for the allocator used by alloc_seq_array */
    }

    /* Zero the new slots (seq_t defaults to zero for integer types; for big ints zero works). */
    if (newcap > cap)
        memset(new_a + cap, 0, (newcap - cap) * sizeof(seq_t));

    a_arr = new_a;
    cap   = newcap;
}

/* Compute a(n) using the recurrence, memoizing results. */
seq_t
a(idx_t n)
{
    /* Make sure storage is large enough. */
    ensure_capacity(n);

    /* Initialize a(0) if this is the first time. */
    if (a_max_n == 0)
        a_arr[0] = (seq_t)1;

    /* If we already know a(n), return it. */
    if (n <= a_max_n)
        return a_arr[n];

    /* Compute missing terms up to n. */
    for (idx_t i = a_max_n + 1; i <= n; ++i)
    {
        seq_t sum = 0;
        /* Sum over squares k^2 ≤ i. */
        for (idx_t k = 1; k * k <= i; ++k)
        {
            sum += a_arr[i - k * k];
        }
        a_arr[i] = sum * 2;   /* multiply by 2 (equivalent to add sum twice). */
    }

    a_max_n = n;
    return a_arr[n];
}
