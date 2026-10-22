/*
 * A344180  Lexicographically earliest infinite sequence such that a(i) = a(j)
 *          => f(i) = f(j) for all i, j >= 0, where f(n) = 0 if n is a
 *          Fibbinary number (A003714), otherwise f(n) = n.
 * https://oeis.org/A344180
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 342, u16 n <= 9826, u32 n <= 9826, u64 n <= 9826,
 *            u128 n <= 9826, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Lexicographically earliest sequence A344180
 *
 * a(n) = 1                               if n is a Fibbinary number
 * a(n) = (# of non‑Fibbinary indices ≤ n) + 1   otherwise
 *
 * That is, all Fibbinary indices receive the value 1.
 * Each time a new non‑Fibbinary index is encountered,
 * it receives the next unused integer value (2, 3, 4, …).
 * The function memoises the computed values so that it works
 * for calls in arbitrary order.  It satisfies the constraints
 * that equal a‑values imply equal f‑values, where
 * f(n) = 0 for Fibbinary n and f(n) = n otherwise. */

#include <stdbool.h>
#include <stdlib.h>

/* Return true iff n is a Fibbinary number (no consecutive ones in binary). */
static inline bool is_fibbinary(idx_t n)
{
    return ( (n & (n >> 1)) == 0 );
}

/* Static data for memoisation.  All variables are zero‑initialised
 * automatically by static storage duration. */
static seq_t *cache   = NULL;   /* computed a(n) values                   */
static idx_t  cap     = 0;       /* allocated number of entries            */
static idx_t  max_n   = 0;       /* entries 0 .. max_n-1 have been computed */
static idx_t  nf_cnt  = 0;       /* number of non‑Fibbinary indices seen   */

seq_t a(idx_t n)
{
    /* Expand the cache if necessary.  The cast is required for both
     * C and C++.  We use realloc which behaves the same in both
     * languages. */
    if (n + 1 > cap)
    {
        seq_t *tmp = (seq_t *)realloc(cache, (size_t)(n + 1) * sizeof(seq_t));
        if (!tmp) /* allocation failed – memory exhausted */
            return (seq_t)0;
        cache = tmp;
        cap   = (idx_t)(n + 1);
    }

    /* Compute values up to index n (inclusive).  For already
     * computed indices nothing is done. */
    for (idx_t i = max_n; i <= n; ++i)
    {
        if (is_fibbinary(i))
            cache[i] = (seq_t)1;
        else
            /* Non‑Fibbinary: assign the next unused value. */
            cache[i] = (seq_t)(++nf_cnt + 1);
    }

    max_n = n + 1;
    return cache[n];
}
