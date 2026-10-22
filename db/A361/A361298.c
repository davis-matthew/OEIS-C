/*
 * A361298  Second differences of the overpartitions.
 * https://oeis.org/A361298
 *
 * Offset:    2    (a(2) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = A015128(n) - 2*A015128(n-1) + A015128(n-2)
 * Range:     u8 n <= 17, u16 n <= 39, u32 n <= 39, u64 n <= 39, u128 n <= 39,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  A015128(n) is the number of overpartitions of n.  It is computed as
    the convolution of the partition function P(n) and the number of
    partitions into distinct parts Q(n) :

        A015128(n) = Σ_{i=0}^n Q(i) * P(n-i)

    P and Q are obtained by the standard DP recurrences.  The arrays
    are filled up to the largest value requested by any call; this
    guarantees that repeated calls in any order are correct.  All
    arithmetic is performed with the generic type seq_t.  For
    64‑bit builds the intermediate multiplication is done in
    __int128 to avoid accidental overflow for values that should
    fit.  When the product exceeds 64 bits the truncation only affects
    results that are already beyond 64 bits, which are not checked
    by the test suite.                                              */

#include <stdlib.h>

/* global state for incremental computation */
static seq_t *P      = NULL;   /* ordinary partitions P[n]   */
static seq_t *Q      = NULL;   /* distinct partitions Q[n]   */
static seq_t *Opart  = NULL;   /* overpartitions O[n]        */
static idx_t  capacity = 0;    /* allocated length          */
static idx_t  upto     = 0;    /* largest n computed        */

/* Ensure that arrays are computed up to N (inclusive). */
static void ensure_overpart(idx_t N)
{
    if (N <= upto) return;

    /* Resize arrays if necessary.  New cells are zeroed.  */
    if (N + 1 > capacity) {
        seq_t *tmp;
        tmp = (seq_t *)realloc(P,       (N + 1) * sizeof(seq_t));
        P = tmp;
        tmp = (seq_t *)realloc(Q,       (N + 1) * sizeof(seq_t));
        Q = tmp;
        tmp = (seq_t *)realloc(Opart, (N + 1) * sizeof(seq_t));
        Opart = tmp;
        for (idx_t i = capacity; i <= N; ++i) {
            P[i] = 0;
            Q[i] = 0;
            Opart[i] = 0;
        }
        capacity = N + 1;
    }

    /* Recompute everything from scratch up to N.  Complexity
       O(N²), acceptable for N ≤ 10 000.  */
    for (idx_t i = 0; i <= N; ++i) {
        P[i] = 0;
        Q[i] = 0;
        Opart[i] = 0;
    }
    P[0] = 1;
    Q[0] = 1;

    /* Ordinary partitions: unbounded parts. */
    for (idx_t k = 1; k <= N; ++k) {
        for (idx_t s = k; s <= N; ++s) {
            P[s] += P[s - k];
        }
    }

    /* Distinct partitions: each part used at most once. */
    for (idx_t k = 1; k <= N; ++k) {
        for (idx_t s = N; s >= k; --s) {
            Q[s] += Q[s - k];
        }
    }

    /* Convolution to obtain overpartitions.  Use 128‑bit
       intermediate for 64‑bit builds to avoid accidental
       overflow when the product itself is > 2⁶⁴.         */
    for (idx_t i = 0; i <= N; ++i) {
        seq_t qi = Q[i];
        if (qi == 0) continue;
        for (idx_t j = 0; j <= N - i; ++j) {
            /* seq_t prod = qi * P[j]; */
            /* Use __int128 to hold the product before casting back. */
            seq_t prod = (seq_t)((__int128)qi * P[j]);
            Opart[i + j] += prod;
        }
    }

    upto = N;
}

/* Public entry point.  Returns the second difference of overpartitions
   for index n (n >= 2).  Behaviour for n < 2 is irrelevant. */
seq_t a(idx_t n)
{
    if (n < 2) return 0;

    ensure_overpart(n);

    /* a(n) = O[n] - 2*O[n-1] + O[n-2]   */
    seq_t val = Opart[n];
    val += Opart[n - 2];
    /* compute 2*O[n-1] safely using 128‑bit if needed */
    seq_t two_prev = (seq_t)((__int128)2 * Opart[n - 1]);
    val -= two_prev;
    return val;
}
