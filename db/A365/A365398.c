/*
 * A365398  Length of the longest subsequence of 1, ..., n on which sigma, the
 *          sum of the divisors of n (A000203), is nondecreasing.
 * https://oeis.org/A365398
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 1309, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   sigma_of
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computation of A365398: length of the longest non‑decreasing
   subsequence of the divisor‑sum sequence σ(1),σ(2),…,σ(n).   */

#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>
#include "../utils/sigma_of.h"

seq_t a(idx_t n)
{
    /* a(1) is the first term; values for n < 1 are irrelevant. */
    if (n < 1) return 0;

    /* ----- static state ----- */
    static seq_t *a_arr   = NULL;          /* a_arr[k] = a(k), 1‑based */
    static size_t a_cap   = 0;             /* capacity of a_arr   */
    static seq_t *tails   = NULL;          /* tails of current LNDS */
    static size_t tails_cap = 0;           /* capacity of tails   */
    static idx_t  processed = 0;           /* largest n processed so far   */
    static idx_t  len = 0;                 /* current LNDS length          */

    /* Ensure a_arr can store up to index n. */
    if (a_cap < (size_t)(n + 1)) {
        seq_t *tmp = (seq_t *)realloc(a_arr, (n + 1) * sizeof(seq_t));
        if (!tmp) return 0;               /* allocation failure */
        a_arr = tmp;
        a_cap = n + 1;
    }

    /* Make sure tails has room for up to n elements. */
    if (tails_cap < (size_t)(n + 1)) {
        size_t new_cap = tails_cap == 0 ? 1 : tails_cap * 2;
        if (new_cap < (size_t)(n + 1)) new_cap = n + 1;
        seq_t *tmp = (seq_t *)realloc(tails, new_cap * sizeof(seq_t));
        if (!tmp) return 0;               /* allocation failure */
        tails = tmp;
        tails_cap = new_cap;
    }

    /* Incrementally extend the LNDS up to the requested n. */
    while (processed < n) {
        ++processed;
        seq_t sigma_val = sigma_of(processed);

        /* Binary search for first tails[i] > sigma_val. */
        idx_t lo = 0, hi = len;
        while (lo < hi) {
            idx_t mid = (lo + hi) >> 1;
            if (tails[mid] > sigma_val)
                hi = mid;
            else
                lo = mid + 1;
        }

        if (lo == len) {          /* extend LNDS */
            tails[len++] = sigma_val;
        } else {                  /* replace to keep minimal tail */
            tails[lo] = sigma_val;
        }

        a_arr[processed] = (seq_t)len;
    }

    return a_arr[n];
}
