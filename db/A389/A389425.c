/*
 * A389425  First differences of A199716.
 * https://oeis.org/A389425
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 4, u16 n <= 100, u32 n <= 100, u64 n <= 100,
 *            u128 n <= 100, big any n
 * Helpers:   isPrimeTrial
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computation: a(n) = A199716(n+1) - A199716(n), where A199716 consists of the
   integers k for which 6k-5 and 6k-1 are composite.  We generate k in
   ascending order, test the composites with isPrimeTrial, store the terms
   and their successive differences.  The function is memoised so that
   subsequent calls (in any order) retrieve previously computed terms. */

#include <stdlib.h>
#include <stdbool.h>
#include <stdint.h>
#include "../utils/isPrimeTrial.h"   /* for isPrimeTrial(seq_t) */

static seq_t *A_terms = NULL;    /* stored A199716 terms (k values) */
static size_t A_size = 0, A_cap = 0;
static seq_t *diffs  = NULL;    /* stored differences a(n) */
static size_t diff_size = 0, diff_cap = 0;
static idx_t  next_k = 1;       /* next candidate k to test */

static void ensure_capacity(seq_t **arr, size_t *cap, size_t needed)
{
    if (needed <= *cap) return;
    size_t newcap = (*cap > 0) ? *cap * 2 : 16;
    while (newcap < needed) newcap <<= 1;
    *arr = (seq_t *)realloc(*arr, newcap * sizeof(seq_t));
    *cap = newcap;
}

seq_t a(idx_t n)
{
    /* need to have diffs[0..n-1] ready */
    if (diff_size < (size_t)n) {
        /* generate A199716 terms until we have at least n+1 terms */
        while (A_size < (size_t)n + 1) {
            seq_t r1 = (seq_t)6 * next_k - 5;
            seq_t r2 = (seq_t)6 * next_k - 1;
            if (!isPrimeTrial(r1) && !isPrimeTrial(r2)) {
                /* store the new term */
                ensure_capacity(&A_terms, &A_cap, A_size + 1);
                A_terms[A_size++] = next_k;

                /* store the difference if we have at least two terms */
                if (A_size >= 2) {
                    seq_t diff = A_terms[A_size - 1] - A_terms[A_size - 2];
                    ensure_capacity(&diffs, &diff_cap, diff_size + 1);
                    diffs[diff_size++] = diff;
                }
            }
            ++next_k;
        }
    }
    return diffs[(size_t)n - 1];
}
