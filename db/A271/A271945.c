/*
 * A271945  Expansion of 4*x^2/(1 - x - 9*x^2 + x^3).
 * https://oeis.org/A271945
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = a(n-1) + 9*a(n-2) - a(n-3), a(0)=0, a(1)=0, a(2)=4
 * Range:     u8 n <= 5, u16 n <= 10, u32 n <= 19, u64 n <= 27, u128 n <= 27,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

/* Static cache for computed terms.  The array is reallocated as needed.
 * `computed` holds the index of the next term that has not yet been
 * computed (i.e., values 0 … computed-1 are valid).  `cap` is the current
 * allocated capacity. */
static seq_t *arr = NULL;
static size_t cap   = 0;
static size_t computed = 0;

/* Ensure the cache can store terms 0..n inclusive. */
static void ensure_capacity(idx_t n)
{
    if (cap <= n) {
        size_t new_cap = cap ? cap * 2 : 4;
        while (new_cap <= (size_t)n) new_cap *= 2;
        seq_t *tmp = (seq_t *)realloc(arr, new_cap * sizeof(seq_t));
        if (!tmp) { /* memory exhausted – abort for safety */ abort(); }
        arr = tmp;
        cap = new_cap;
    }
}

/* Return a(n).  Works for any order of calls. */
seq_t a(idx_t n)
{
    /* Handle the base values directly.  This also makes the function
     * correct when called with n<3. */
    if (n == 0) return (seq_t)0;
    if (n == 1) return (seq_t)0;
    if (n == 2) return (seq_t)4;

    /* On the first call initialise the base values. */
    if (computed == 0) {
        ensure_capacity(2);
        arr[0] = (seq_t)0;
        arr[1] = (seq_t)0;
        arr[2] = (seq_t)4;
        computed = 3; /* terms 0,1,2 now available */
    }

    /* If we already have the desired term, just return it. */
    if ((idx_t)computed > n) return arr[n];

    /* Compute missing terms up to n using the recurrence. */
    ensure_capacity(n);
    for (size_t i = computed; i <= (size_t)n; ++i) {
        arr[i] = arr[i-1] + (seq_t)9 * arr[i-2] - arr[i-3];
    }
    computed = (size_t)n + 1;
    return arr[n];
}
