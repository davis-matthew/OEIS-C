/*
 * A245505  Number of endofunctions f on [n] such that f^9(i) = f(i) for all i
 *          in [n].
 * https://oeis.org/A245505
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Σ_{k=0..n} C(n,k) * k^{n-k} * A_8(k)
 * Range:     u8 n <= 4, u16 n <= 6, u32 n <= 11, u64 n <= 18, u128 n <= 30,
 *            big any n
 * Helpers:   seq_pow
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* where A_8(k) = number of permutations on k with all cycle lengths
   dividing 8 (i.e. in {1,2,4,8}). */

/* Auxiliary routine available in the build: 
   seq_pow(seq_t base, idx_t exp)                       */
#include <stdlib.h>
#include <stdint.h>
#include <stddef.h>
#include "../utils/seq_pow.h"

/* Static cache for p(k) = A_8(k) */
static seq_t *p_seq = NULL;      /* p(k) values   */
static idx_t  p_max = 0;         /* largest k computed */

/* Static cache for a(n) */
static seq_t *a_seq = NULL;      /* a(n) values   */
static idx_t  a_max = 0;         /* largest n computed */

/* -------------------------------------------------------------- */
/* Compute p(k) up to n (inclusive).  p(0) = 1.                  */
static void
ensure_p(idx_t n)
{
    if (p_seq && p_max >= n)
        return;                  /* already up to date */

    /* Allocate or enlarge array */
    size_t new_cap = (size_t)(n + 1);
    seq_t *tmp = (seq_t *)realloc(p_seq, new_cap * sizeof(seq_t));
    if (!tmp) return;            /* silently abort on OOM */

    p_seq = tmp;

    /* Ensure p(0) is defined */
    if (p_max == 0 && p_seq[0] == 0)
        p_seq[0] = 1;

    for (idx_t k = p_max + 1; k <= n; ++k) {
        seq_t val = 0;

        /* Cycle lengths allowed: 1, 2, 4, 8 */
        /* d = 1 */
        {
            seq_t prod = 1;      /* (k-1)!/(k-1)! = 1 */
            val += prod * p_seq[k - 1];
        }

        /* d = 2 */
        if (k >= 2) {
            seq_t prod = 1;
            for (idx_t j = k - 2 + 1; j <= k - 1; ++j)
                prod *= (seq_t)j;   /* = (k-1)!/(k-2)! */
            val += prod * p_seq[k - 2];
        }

        /* d = 4 */
        if (k >= 4) {
            seq_t prod = 1;
            for (idx_t j = k - 4 + 1; j <= k - 1; ++j)
                prod *= (seq_t)j;   /* = (k-1)!/(k-4)! */
            val += prod * p_seq[k - 4];
        }

        /* d = 8 */
        if (k >= 8) {
            seq_t prod = 1;
            for (idx_t j = k - 8 + 1; j <= k - 1; ++j)
                prod *= (seq_t)j;   /* = (k-1)!/(k-8)! */
            val += prod * p_seq[k - 8];
        }

        p_seq[k] = val;
    }

    p_max = n;
}

/* -------------------------------------------------------------- */
/* Compute a(n) up to n (inclusive).  a(0) = 1.  Requires p(k). */
static void
ensure_a(idx_t n)
{
    if (a_seq && a_max >= n)
        return;                  /* already up to date */

    /* Ensure p(•) is available up to n */
    ensure_p(n);

    /* Allocate or enlarge array for a(n) */
    size_t new_cap = (size_t)(n + 1);
    seq_t *tmp = (seq_t *)realloc(a_seq, new_cap * sizeof(seq_t));
    if (!tmp) return;
    a_seq = tmp;

    if (a_max == 0 && a_seq[0] == 0)
        a_seq[0] = 1;

    for (idx_t m = a_max + 1; m <= n; ++m) {
        if (m == 0) {
            a_seq[0] = 1;
            continue;
        }

        seq_t total = 0;
        seq_t binom  = 1;         /* C(m,0) */

        for (idx_t k = 1; k <= m; ++k) {
            /* Update binomial coefficient: C(m,k) */
            binom = (binom * (seq_t)(m - k + 1)) / (seq_t)k;

            /* k^{m-k} */
            seq_t powk = seq_pow((seq_t)k, m - k);

            /* Term = C(m,k) * p(k) * k^{m-k} */
            total += binom * p_seq[k] * powk;
        }

        a_seq[m] = total;
    }

    a_max = n;
}

/* -------------------------------------------------------------- */
/* Public function required by the challenge */
seq_t
a(idx_t n)
{
    ensure_a(n);
    return a_seq[n];
}
