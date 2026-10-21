/*
 * A293766  Solution of the complementary equation a(n) = a(n-1) + a(n-2) +
 *          b(n-1) + 3, where a(0) = 1, a(1) = 3, b(0) = 2, b(1) = 4.
 * https://oeis.org/A293766
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = a(n-1) + a(n-2) + b(n-1) + 3 a(0) = 1, a(1) = 3 b(0) = 2,
 *            b(1) = 4 The complementary sequence b is the smallest positive
 *            integer not already used by a and b so far
 * Range:     u8 n <= 7, u16 n <= 18, u32 n <= 35, u64 n <= 35, u128 n <= 35,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdbool.h>

/* Forward declaration */
static bool contains_a(seq_t, idx_t);

/* Dynamically growable arrays of a(n) and b(n) */
static seq_t *a_vals = NULL;
static seq_t *b_vals = NULL;
static idx_t   allocated = 0;   /* number of allocated slots */
static idx_t   max_index = 0;   /* highest computed index + 1 */

/* Ensure capacity for at least n+1 elements */
static void ensure_capacity(idx_t n)
{
    if (n < allocated) return;
    idx_t new_cap = allocated ? allocated : 4;
    while (new_cap <= n) new_cap <<= 1;
    a_vals = (seq_t *)realloc(a_vals, new_cap * sizeof(seq_t));
    b_vals = (seq_t *)realloc(b_vals, new_cap * sizeof(seq_t));
    allocated = new_cap;
}

/* Binary search: does a[0..upto] contain val?  */
static bool contains_a(seq_t val, idx_t upto)
{
    idx_t lo = 0, hi = upto;
    while (lo <= hi) {
        idx_t mid = (lo + hi) >> 1;
        if (a_vals[mid] == val) return true;
        if (a_vals[mid] < val) lo = mid + 1;
        else hi = mid - 1;
    }
    return false;
}

seq_t a(idx_t n)
{
    /* Initialisation on first call */
    if (max_index == 0) {
        ensure_capacity(1);
        a_vals[0] = (seq_t)1;
        b_vals[0] = (seq_t)2;
        max_index = 1;
    }

    /* Compute up to desired n */
    while (max_index <= n) {
        if (max_index == 1) {
            /* Handle n==1 */
            ensure_capacity(2);
            a_vals[1] = (seq_t)3;
            b_vals[1] = (seq_t)4;
            max_index = 2;
        } else {
            /* General recurrence */
            seq_t new_a = a_vals[max_index - 1] +
                          a_vals[max_index - 2] +
                          b_vals[max_index - 1] + (seq_t)3;

            /* Find next complementary b value */
            seq_t candidate = b_vals[max_index - 1] + (seq_t)1;
            while (contains_a(candidate, max_index - 1))
                ++candidate;

            ensure_capacity(max_index + 1);
            a_vals[max_index] = new_a;
            b_vals[max_index] = candidate;
            ++max_index;
        }
    }

    return a_vals[n];
}
