/*
 * A299041  Irregular table: T(n,k) equals the number of alignments of length
 *          k of n strings each of length 3.
 * https://oeis.org/A299041
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, tabf, easy
 * Formula:   not stated
 * Range:     u8 n <= 7, u16 n <= 15, u32 n <= 40, u64 n <= 40, u128 n <= 40,
 *            big any n
 * Helpers:   binom_k3
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  Computes a(n) from OEIS A299041.
 *
 *  The sequence is defined by selecting one entry from each row of the
 *  irregular table T(n,k) (n strings of length 3, alignment length k).
 *  The selected entry is obtained via an irregular pattern that can be
 *  described as follows:
 *
 *  * For n = 1 or 2  a(n) = 1.
 *  * For all other n, let i_t = 2 + t(3t+5)/2  (t = 0,1,2,…).  
 *    These indices i_t (2,6,13,23,…) are the positions where a(n)=1.
 *    If n = i_t, return 1.
 *  * Otherwise, let t be the largest integer with i_t < n (so i_t is the
 *    preceding 1‑position); set m = t + 2 and k = n - i_t + 3.  Then
 *    a(n) = T(m,k), where T(m,k) satisfies  
 *
 *        T(1,3) = 1 ,  T(n,k)=0 for k<3 or k>3n
 *        T(n,k) = C(k,3) * [ T(n-1,k) + 3*T(n-1,k-1) +
 *                           3*T(n-1,k-2) + T(n-1,k-3) ]   (n≥2)
 *
 *  The algorithm searches for the correct block in which n lies, then
 *  evaluates T(m,k) by dynamic programming.
 *
 *  The implementation follows the rules of the specification: uses only
 *  integer arithmetic and the types seq_t and idx_t provided by the
 *  build system.
 */

#include <stdint.h>
#include <stdlib.h>
#include "../utils/binom_k3.h"

/* Compute T(m,k) by recurrence,  m>=1, 3<=k<=3*m */
static seq_t compute_T(idx_t m, idx_t k)
{
    if (k < 3 || k > 3*m) return 0;
    if (m == 1) return (k == 3 ? 1 : 0);

    /* Allocate two rows of length k+1 */
    seq_t *prev = (seq_t *)calloc(k+1, sizeof(seq_t));
    seq_t *cur  = (seq_t *)calloc(k+1, sizeof(seq_t));
    if (!prev || !cur) {
        /* allocation failed; return 0 as a safe fallback */
        free(prev);
        free(cur);
        return 0;
    }

    /* Row for n=1 */
    prev[3] = 1;  /* all other entries remain 0 */

    for (idx_t n = 2; n <= m; ++n) {
        idx_t kmax = (3*n < k) ? 3*n : k;   /* upper limit for this row */

        /* clear current row */
        for (idx_t kk = 3; kk <= kmax; ++kk)
            cur[kk] = 0;

        for (idx_t kk = 3; kk <= kmax; ++kk) {
            seq_t bin = binom_k3(kk);
            seq_t sum = 0;

            /* T(n-1, kk) */
            sum += prev[kk];

            /* 3*T(n-1, kk-1) */
            if (kk-1 > 0) sum += 3*prev[kk-1];

            /* 3*T(n-1, kk-2) */
            if (kk-2 > 0) sum += 3*prev[kk-2];

            /* T(n-1, kk-3) */
            if (kk-3 > 0) sum += prev[kk-3];

            cur[kk] = bin * sum;
        }

        /* swap rows for next iteration */
        seq_t *tmp = prev;
        prev = cur;
        cur = tmp;
    }

    seq_t result = prev[k];   /* last computed row is stored in prev */
    free(prev);
    free(cur);
    return result;
}

seq_t a(idx_t n)
{
    /* Base cases with a(1)=a(2)=1 */
    if (n <= 2) return 1;

    /* Search for the preceding 1-position i_t */
    idx_t prev_i = 0;
    idx_t prev_t = 0;

    for (idx_t t = 0; ; ++t) {
        /* i_t = 2 + t(3t+5)/2 */
        idx_t i_t = 2 + ((t * (3*t + 5)) >> 1);   /* divide by 2 safely */
        if (n == i_t) return 1;                  /* at a 1-position */
        if (i_t > n) break;                      /* past the current block */
        prev_i = i_t;
        prev_t = t;
    }

    idx_t m = prev_t + 2;                         /* number of strings */
    idx_t k = n - prev_i + 3;                     /* alignment length    */

    return compute_T(m, k);
}
