/*
 * A333375  Start with a(0) = a(1) = 1. If a(n) = n is the rightmost term
 *          defined so far, let a(m) = m := n + a(n-1). If the terms between
 *          a(n) and a(m) are undefined, let a(n+1) = a(n) + a(m) and if m >
 *          n+1, a(m-1) = a(n+1) + a(m).
 * https://oeis.org/A333375
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 8, u16 n <= 21, u32 n <= 44, u64 n <= 44, u128 n <= 44,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* 
   SEQ A333375: a(0) = a(1) = 1.
   Recursive construction:
   * Let L be the rightmost defined index.
   * If a[L] == L, let R = L + a[L-1] and set a[R] = R.
   * Then fill all indices between L and R with the Fibonacci‑like rule
     adjacent to the boundaries:
        i = L; j = R;
        while (i+1 < j) {
            a[i+1] = a[i] + a[j];
            if (i+1 == j-1) break;
            a[j-1] = a[i+1] + a[j];
            i++; j--;
        }
   The process stops when the requested index n is defined.
*/

#include <stdlib.h>
#include <stdbool.h>

static seq_t *seq = NULL;         /* sequence values                  */
static bool  *defined = NULL;      /* whether seq[i] has been set      */
static size_t  cap = 0;            /* allocated capacity               */
static size_t  last_defined = 1;   /* largest index defined so far     */

/* Ensure capacity for indices up to `need` (inclusive).              */
static void ensure_capacity(size_t need)
{
    if (need < cap) return;
    size_t newcap = cap ? cap * 2 : 8;
    while (newcap <= need) newcap <<= 1;
    seq      = (seq_t*)realloc(seq,      newcap * sizeof(seq_t));
    defined  = (bool*)realloc(defined, newcap * sizeof(bool));
    if (!seq || !defined) exit(1);
    for (size_t i = cap; i < newcap; ++i) defined[i] = false;
    cap = newcap;
}

/* Main sequence function.                                          */
seq_t a(idx_t n)
{
    /* Lazy initialization of first two terms.                      */
    if (!seq) {
        ensure_capacity(2);
        seq[0] = (seq_t)1; defined[0] = true;
        seq[1] = (seq_t)1; defined[1] = true;
        last_defined = 1;
    }

    if (n <= 1) return seq[n];

    /* Generate terms until the requested index is defined.         */
    while (last_defined < (size_t)n) {
        /* Ensure the predecessor of the rightmost index exists.  */
        if (!defined[last_defined-1]) {
            /* In a correct run this never happens.  */
            exit(1);
        }

        /* Compute the next boundary R = L + a[L-1].                */
        size_t L = last_defined;
        size_t a_prev = (size_t)seq[L-1];
        size_t R = L + a_prev;

        /* Allocate space for new indices up to R.                   */
        ensure_capacity(R);

        seq[R] = (seq_t)R;
        defined[R] = true;

        /* Fill all indices between L and R.                         */
        size_t i = L, j = R;
        while (i + 1 < j) {
            seq[i+1] = seq[i] + seq[j];
            defined[i+1] = true;
            if (i+1 == j-1) break;   /* last element, only one slot */
            seq[j-1] = seq[i+1] + seq[j];
            defined[j-1] = true;
            i++; j--;
        }

        /* Update the rightmost defined index.                      */
        last_defined = R;
    }

    return seq[n];
}
