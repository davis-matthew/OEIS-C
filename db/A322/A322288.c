/*
 * A322288  Indices n for which the partial sums of sin(k) (0 <= k <= n) reach
 *          a new minimum.
 * https://oeis.org/A322288
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 6, u16 n <= 48, u32 n <= 48, u64 n <= 48, u128 n <= 48,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computes indices n at which the partial sums of sin(k) (0 ≤ k ≤ n)
   attain a new minimum.  The algorithm takes successive values of k,
   updates the running sum, and records a new index whenever the sum
   goes below the current minimum.  The result is memoised across
   calls. */
#include <stdint.h>
#include <stdbool.h>
#include <stdlib.h>
#include <math.h>

/* Ensure the array has room for at least new_len elements. */
static void ensure_capacity(seq_t **arr, size_t *cap, size_t new_len)
{
    if (*cap >= new_len)
        return;
    size_t new_cap = (*cap > 0) ? (*cap << 1) : 16;
    seq_t *tmp = (seq_t *)realloc(*arr, new_cap * sizeof(seq_t));
    if (!tmp)
        abort();                 /* allocation failure – terminate */
    *arr = tmp;
    *cap = new_cap;
}

seq_t a(idx_t n)
{
    /* Static state retained between calls. */
    static seq_t *mins   = NULL;   /* indices of new minima */
    static size_t cap    = 0;      /* capacity of mins                           */
    static size_t len    = 0;      /* number of minima stored                    */
    static long double sum    = 0.0L; /* running partial sum of sin(k)             */
    static long double min_sum= 0.0L; /* smallest sum seen so far                  */
    static idx_t last_k  = 0;      /* last k that was processed                  */

    /* Initialise on first invocation. */
    if (!mins) {
        ensure_capacity(&mins, &cap, 16);
        len = 1;                    /* a(0) = 0 */
        mins[0] = (seq_t)0;
        sum = sinl((long double)0);
        min_sum = sum;
        last_k = 0;
    }

    /* Extend computation until we have at least n+1 minima recorded. */
    while (len <= (size_t)n) {
        idx_t k = last_k + 1;
        long double s = sinl((long double)k);
        sum += s;
        if (sum < min_sum) {
            min_sum = sum;
            ensure_capacity(&mins, &cap, len + 1);
            mins[len++] = (seq_t)k;   /* record new minimum index */
        }
        last_k = k;
    }

    return mins[(size_t)n];
}
