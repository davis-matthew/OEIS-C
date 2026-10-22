/*
 * A361656  Number of odd-length integer partitions of n with integer mean.
 * https://oeis.org/A361656
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Σ_{d|n, d odd} p(n,d) where p(n,d) is the number of
 *            partitions of n into exactly d parts. a(0) = 0
 * Range:     u8 n <= 26, u16 n <= 59, u32 n <= 160, u64 n <= 487,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

/* --------------------------------------------------------------------- */
/*  Static state                                                     */
/* --------------------------------------------------------------------- */
static seq_t *A     = NULL;   /* final results for 0..Nmax               */
static seq_t *prev  = NULL;   /* partitions into k-1 parts              */
static seq_t *cur   = NULL;   /* partitions into k   parts              */
static idx_t   Nmax  = 0;      /* largest n that has been computed so far */
static bool    ready = false; /* true once the tables are built */

/* --------------------------------------------------------------------- */
/*  Rebuild the tables up to a given n                                 */
/* --------------------------------------------------------------------- */
static void recompute(idx_t n)
{
    if (ready && Nmax >= n) return;          /* already computed up to n */

    /* free any previous allocation */
    if (A)   free(A);
    if (prev) free(prev);
    if (cur)  free(cur);

    Nmax = n;
    A    = (seq_t *)malloc((Nmax + 1) * sizeof(seq_t));
    prev = (seq_t *)malloc((Nmax + 1) * sizeof(seq_t));
    cur  = (seq_t *)malloc((Nmax + 1) * sizeof(seq_t));

    if (!A || !prev || !cur) {                 /* allocation failed */
        abort();                               /* terminate program */
    }

    /* initialise all entries to zero */
    memset(A,   0, (Nmax + 1) * sizeof(seq_t));
    memset(prev,0, (Nmax + 1) * sizeof(seq_t));
    memset(cur,  0, (Nmax + 1) * sizeof(seq_t));

    /* base case: one partition of 0 into 0 parts */
    prev[0] = (seq_t)1;

    /* dynamic programming over the number of parts k */
    for (idx_t k = 1; k <= Nmax; ++k) {
        /* clear first k entries of cur for the new k */
        for (idx_t i = 0; i < k; ++i) cur[i] = (seq_t)0;

        /* compute partitions of i into exactly k parts */
        for (idx_t i = k; i <= Nmax; ++i) {
            cur[i] = cur[i - k] + prev[i - 1];
        }

        /* accumulate results for this odd divisor k */
        if (k & 1ULL) {                       /* k is odd */
            for (idx_t i = k; i <= Nmax; i += k) {
                A[i] = A[i] + cur[i];
            }
        }

        /* prepare for next iteration: prev <- cur, cur <- old prev */
        seq_t *tmp  = prev;
        prev       = cur;
        cur        = tmp;
    }

    ready = true;
}

/* --------------------------------------------------------------------- */
/*  Public interface:  seq_t a(idx_t n)                                 */
/* --------------------------------------------------------------------- */
seq_t a(idx_t n)
{
    /* ensure we have computed up to n */
    recompute(n);
    return A[n];
}
