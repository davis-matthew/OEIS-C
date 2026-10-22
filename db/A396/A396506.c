/*
 * A396506  A sequence constructed by greedily sampling the probability
 *          distribution given by 1/log_2(i+1)^2 - 1/log_2(i+2)^2 to minimize
 *          discrepancy.
 * https://oeis.org/A396506
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 8163, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  A396506 – Greedy sequence defined by
    a(n) = argmax_k { n*p(k) - c(k) },  n>=1
    where p(k) = 1/(log₂(k+1)²) - 1/(log₂(k+2)²)
    and c(k) is the number of occurrences of k among a(1) … a(n‑1). */

#include <stdlib.h>
#include <math.h>
#include <stdbool.h>

/* ----- static storage for the sequence ----- */
static seq_t *seq      = NULL;      /* array of produced terms */
static size_t seq_cap  = 0;         /* allocated capacity of seq array */
static size_t seq_len  = 0;         /* number of terms currently stored */

/* ----- static storage for occurrence counts ----- */
static idx_t *counts   = NULL;      /* counts[k] = # of times k has appeared (1‑based) */
static size_t counts_cap = 0;       /* allocated capacity of counts array */

/* ----- static storage for probability values ----- */
static long double *p_val = NULL;    /* p_val[k] = probability of k (1‑based) */
static size_t p_cap = 0;            /* allocated capacity of p_val array */

/* --------- helper to grow the seq array --------- */
static void grow_seq(size_t needed)
{
    if (seq_cap < needed) {
        size_t newcap = seq_cap ? seq_cap * 2 : 64;
        if (newcap < needed) newcap = needed;
        seq = (seq_t *)realloc(seq, newcap * sizeof(seq_t));
        seq_cap = newcap;
    }
}

/* --------- helper to grow the counts array --------- */
static void grow_counts(size_t needed)
{
    if (counts_cap < needed) {
        size_t old_cap = counts_cap;
        size_t newcap = old_cap ? old_cap * 2 : 64;
        if (newcap < needed) newcap = needed;
        counts = (idx_t *)realloc(counts, newcap * sizeof(idx_t));
        for (size_t i = old_cap; i < newcap; ++i) counts[i] = 0;
        counts_cap = newcap;
    }
}

/* --------- helper to grow the p_val array --------- */
static void grow_p(size_t needed)
{
    if (p_cap < needed) {
        size_t old_cap = p_cap;
        size_t newcap = old_cap ? old_cap * 2 : 64;
        if (newcap < needed) newcap = needed;
        p_val = (long double *)realloc(p_val, newcap * sizeof(long double));
        for (size_t i = old_cap; i < newcap; ++i) {
            long double t1 = log2l((long double)(i + 1L));   /* log₂(i+1) */
            long double t2 = log2l((long double)(i + 2L));   /* log₂(i+2) */
            p_val[i] = 1.0L / (t1 * t1) - 1.0L / (t2 * t2);
        }
        p_cap = newcap;
    }
}

/* ----------------- main function ----------------- */
seq_t a(idx_t n)
{
    if (n < 1) return 0;          /* undefined for indices < 1 */

    /* Generate terms up to n if they are not already available */
    while (seq_len < (size_t)n) {
        idx_t idx = seq_len + 1;          /* current position (1‑based) */

        /* Ensure auxiliary arrays are large enough for this position */
        grow_counts(idx + 1);              /* counts up to idx+1 needed during scan */
        grow_p(idx + 1);                   /* p_val up to idx+1 computed on demand */

        /* Find best_k by scanning all possible k values */
        idx_t best_k = 1;
        long double best_val = -1e200L;   /* very small value */

        for (idx_t k = 1; k <= idx; ++k) {
            long double val = (long double)idx * p_val[k] - (long double)counts[k];
            if (val > best_val) {
                best_val = val;
                best_k = k;
            }
        }

        /* Append the chosen term to the sequence */
        grow_seq(seq_len + 1);
        seq[seq_len] = (seq_t)best_k;   /* store as seq_t (may be big‑int) */
        seq_len++;

        /* Update the occurrence count for the chosen term */
        grow_counts(best_k + 1);
        counts[best_k]++;
    }

    return seq[n - 1];             /* zero‑based index in the array */
}
