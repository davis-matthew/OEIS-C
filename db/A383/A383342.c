/*
 * A383342  Lexicographically earliest infinite sequence of distinct positive
 *          integers such that the number following any consecutive pair x, y
 *          of terms is the smallest novel number divisible by R(x,y) =
 *          rad(x*y)/rad(gcd(x,y)).
 * https://oeis.org/A383342
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 26, u16 n <= 170, u32 n <= 1204, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   gcd, rad
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A383342: Lexicographically earliest infinite sequence of distinct positive integers
   such that for any consecutive pair x, y of terms the following number is the smallest
   novel positive integer divisible by R(x,y) = rad(x*y)/rad(gcd(x,y))
   (equivalently, R(x,y) = rad(x)/rad(gcd(x,y)) * rad(y)/rad(gcd(x,y)) ).
   The function computes terms lazily, caching the results so that calls in any order
   are correct and efficient. */

#include <stdlib.h>
#include <stdbool.h>
#include <stdint.h>

#include "../utils/rad.h"      /* rad(idx_t m) -> seq_t */
#include "../utils/gcd.h"     /* gcd(seq_t a, seq_t b) -> seq_t */

/* Check whether a value already occurs in the cached sequence. */
static bool used_in_seq(const seq_t *seq, size_t len, seq_t val)
{
    for (size_t i = 0; i < len; ++i)
        if (seq[i] == val)
            return true;
    return false;
}

/* Ensure the dynamic array can hold at least 'need' elements. */
static void ensure_capacity(seq_t **arr, size_t *cap, size_t need)
{
    if (*cap < need) {
        size_t new_cap = (*cap == 0) ? 4 : *cap;
        while (new_cap < need)
            new_cap <<= 1;
        seq_t *tmp = (seq_t *)realloc(*arr, new_cap * sizeof(seq_t));
        if (!tmp) abort();          /* allocation failure - terminate */
        *arr = tmp;
        *cap = new_cap;
    }
}

/* Main function required by the problem statement. */
seq_t a(idx_t n)
{
    if (n < 1) return (seq_t)0;     /* undefined for n < 1 */

    /* Static storage for the sequence. */
    static seq_t *seq = NULL;
    static size_t len = 0;          /* number of terms currently stored */
    static size_t cap = 0;          /* allocated capacity in terms */

    /* Initialize with the first two known terms. */
    if (len == 0) {
        ensure_capacity(&seq, &cap, 4);
        seq[0] = (seq_t)1;
        seq[1] = (seq_t)2;
        len = 2;
    }

    /* Generate terms up to the requested index. */
    while (len < n) {
        seq_t prev = seq[len - 2];
        seq_t curr = seq[len - 1];

        /* Compute R(x,y) = rad(x)/rad(gcd) * rad(y)/rad(gcd). */
        seq_t rad_prev  = rad((idx_t)prev);
        seq_t rad_curr  = rad((idx_t)curr);
        seq_t rad_gcd   = rad((idx_t)gcd(prev, curr));

        seq_t t1 = rad_prev / rad_gcd;   /* rad(prev)/rad(gcd) */
        seq_t t2 = rad_curr / rad_gcd;   /* rad(curr)/rad(gcd) */
        seq_t R = t1 * t2;               /* R(x,y) */

        if (R == 0) R = 1;               /* safety guard, should never happen */

        /* Find the smallest novel multiple of R. */
        seq_t candidate = R;
        while (used_in_seq(seq, len, candidate))
            candidate += R;

        /* Append the new term to the cache. */
        ensure_capacity(&seq, &cap, len + 1);
        seq[len] = candidate;
        ++len;
    }

    return seq[n - 1];
}
