/*
 * A304783  Expansion of Product_{k>=1} (1 - x^k)^q(k), where q(k) = number of
 *          partitions of k into distinct parts (A000009).
 * https://oeis.org/A304783
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign
 * Formula:   a(n) = coefficient of x^n in * Product_{k>=1} (1 -
 *            x^k)^{A000009(k)} * * Let S(m) = sum_{d|m} d·A000009(d). * Then
 *            the recurrence is * a(0) = 1, * n·a(n) = -∑_{k=1}^n S(k)·a(n‑k),
 *            for n≥1. * The division is exact by construction
 * Range:     i8 n <= 15, i16 n <= 38, i32 n <= 53, i64 n <= 53, i128 n <= 53,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdint.h>
#include <stdlib.h>

static idx_t max_comp = 0;
static seq_t *q_val  = NULL;   /* A000009(k) */
static seq_t *S_val  = NULL;   /* sum_{d|k} d·q(d) */
static seq_t *a_val  = NULL;   /* a(k) */

/* Return a(n) ----------------------------------------------------------- */
seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)1;

    if (n > max_comp) {
        size_t sz = (size_t)n + 1;

        /* Allocate / grow workspace */
        q_val = (seq_t *)realloc(q_val, sz * sizeof(seq_t));
        S_val = (seq_t *)realloc(S_val, sz * sizeof(seq_t));
        a_val = (seq_t *)realloc(a_val, sz * sizeof(seq_t));

        /* initialise */
        for (size_t i = 0; i <= n; ++i) {
            q_val[i] = 0;
            S_val[i] = 0;
            a_val[i] = 0;
        }
        q_val[0] = (seq_t)1;

        /* Compute A000009(k) by DP (partitions into distinct parts) */
        for (idx_t part = 1; part <= n; ++part) {
            for (idx_t s = n; s >= part; --s) {
                q_val[s] += q_val[s - part];
            }
        }

        /* Compute S(m) = sum_{d|m} d * q(d) */
        for (idx_t d = 1; d <= n; ++d) {
            seq_t val = (seq_t)d * q_val[d];
            for (idx_t m = d; m <= n; m += d) {
                S_val[m] += val;
            }
        }

        /* Compute a(k) using the recurrence */
        a_val[0] = (seq_t)1;
        for (idx_t i = 1; i <= n; ++i) {
            seq_t sum = 0;
            for (idx_t k = 1; k <= i; ++k) {
                sum += -(S_val[k]) * a_val[i - k];
            }
            seq_t divisor = (seq_t)i;
            a_val[i] = sum / divisor;   /* exact division */
        }

        max_comp = n;
    }

    return a_val[n];
}
