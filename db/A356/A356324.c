/*
 * A356324  a(n) is the first split point of the permutation p if p is the
 *          n-th permutation (in lexicographic order (A030298 prepended by the
 *          empty permutation)), or zero if it has no split point.
 * https://oeis.org/A356324
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, tabf
 * Formula:   not stated
 * Range:     u8 n <= 86, u16 n <= 86, u32 n <= 86, u64 n <= 86, u128 n <= 86,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computes the first split point of the n‑th permutation in the
   concatenated list of all permutations (empty permutation at n = 0).
   Return 0 if the permutation has no split point. */
#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>
#include <limits.h>

#define MAX_K 25   /* sufficient for n ≤ 10000 and small danger zone */

/* external linkage as required */
seq_t a(idx_t n)
{
    /* empty permutation */
    if (n == 0) return (seq_t)0;

    /* Find the size k of the permutation that contains the n‑th element.
       cntPrev holds the total number of permutations of smaller sizes. */
    idx_t cntPrev = 0;
    idx_t fact = 1;          /* 0! = 1 */
    size_t k = 0;

    while (1) {
        if (n < cntPrev + fact) break;
        cntPrev += fact;
        ++k;
        if (k > MAX_K) break;            /* out of bounds – give up */
        fact *= (idx_t)k;                /* compute (k)! for next round */
    }
    if (k == 0 || k > MAX_K) return (seq_t)0;

    /* rank of the permutation inside all permutations of size k */
    idx_t rank = n - cntPrev;   /* 0‑based within size‑k permutations */

    /* Pre‑compute factorials up to k (k! fits in idx_t for k ≤ MAX_K). */
    idx_t fact_arr[MAX_K + 1];
    fact_arr[0] = 1;
    for (size_t i = 1; i <= k; ++i) {
        fact_arr[i] = fact_arr[i-1] * (idx_t)i;
    }

    /* Build the rank‑th lexicographic permutation of [1..k]. */
    int avail[MAX_K];
    for (size_t i = 0; i < k; ++i) avail[i] = (int)(i + 1);
    int perm[MAX_K];

    idx_t r = rank;
    for (size_t pos = 0; pos < k; ++pos) {
        idx_t div = fact_arr[k-1-pos];      /* (k-1-pos)! */
        idx_t q = r / div;
        r = r % div;
        perm[pos] = avail[(size_t)q];

        /* remove used element from avail */
        for (size_t j = (size_t)q; j + 1 < k - pos; ++j) {
            avail[j] = avail[j+1];
        }
    }

    if (k <= 1) return (seq_t)0;   /* no possible split point */

    /* Compute suffix minima: min_suf[i] = min of perm[i..k-1] */
    int min_suf[MAX_K];
    for (int i = (int)k-1; i >= 0; --i) {
        if (i == (int)k-1) min_suf[i] = perm[i];
        else {
            int m = perm[i];
            min_suf[i] = (m < min_suf[i+1]) ? m : min_suf[i+1];
        }
    }

    /* Scan for the first split point. */
    int max_prefix = perm[0];
    for (size_t i = 1; i < k; ++i) {
        if (max_prefix < min_suf[i])   /* split after position i */
            return (seq_t)i;           /* i is 1‑based split point */
        if (perm[i] > max_prefix) max_prefix = perm[i];
    }
    return (seq_t)0;                   /* no split point found */
}
