/*
 * A343762  a(1) = 1; a(n) = -Sum_{k=1..n} a(k/gcd(n,k)).
 * https://oeis.org/A343762
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  sign
 * Formula:   not stated
 * Range:     i8 n <= 16, i16 n <= 36, i32 n <= 47, i64 n <= 47, i128 n <= 47,
 *            big any n
 * Helpers:   gcd
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  OEIS A343762
    a(1) = 1;
    a(n) = -∑_{k=1..n} a(k / gcd(n,k))    for n ≥ 2
    Equivalent: a(n) = -∑_{d|n}  S(d)
    where S(d) = ∑_{k=1..d, gcd(k,d)=1} a(k)
    (for d > 1, S(d) does not depend on a(d) itself, so the recurrence
     can be evaluated iteratively).
*/

#include <stdint.h>
#include <stdlib.h>
#include "../utils/gcd.h"

static seq_t *a_arr = NULL;          /* a[1..max_n]   */
static seq_t *S_arr = NULL;          /* S[1..max_n]   */
static idx_t   max_n = 0;            /* highest n computed */

/* Expand the tables up to the requested n (inclusive). */
static void ensure_capacity(idx_t n)
{
    if (n <= max_n) return;

    /* Reallocate arrays to size n+1 (ignoring index 0). */
    a_arr = (seq_t *)realloc(a_arr, (n + 1) * sizeof(seq_t));
    S_arr = (seq_t *)realloc(S_arr, (n + 1) * sizeof(seq_t));

    for (idx_t i = max_n + 1; i <= n; ++i) {
        if (i == 1) {
            a_arr[1] = (seq_t)1;
            S_arr[1] = (seq_t)1;
            continue;
        }

        /* 1. Compute S[i] = Σ_{k=1..i-1, gcd(k,i)=1} a[k]    */
        seq_t sumS = (seq_t)0;
        for (idx_t k = 1; k < i; ++k) {
            if (gcd((seq_t)k, (seq_t)i) == (seq_t)1)
                sumS += a_arr[k];
        }
        S_arr[i] = sumS;

        /* 2. Compute a[i] = -Σ_{d|i} S[d]                       */
        seq_t a_val = (seq_t)0;
        for (idx_t d = 1; d * d <= i; ++d) {
            if (i % d == 0) {
                a_val -= S_arr[d];
                if (d != i / d)
                    a_val -= S_arr[i / d];
            }
        }
        a_arr[i] = a_val;
    }
    max_n = n;
}

seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0;   /* behaviour below the offset is undefined */
    ensure_capacity(n);
    return a_arr[n];
}
