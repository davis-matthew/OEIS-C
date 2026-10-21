/*
 * A241725  Number of compositions of n into distinct parts with exactly six
 *          descents.
 * https://oeis.org/A241725
 *
 * Offset:    28    (a(28) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = sum_{k>=7} <k,6> * p_k_distinct(n) where <k,6> is the
 *            Eulerian number counting permutations of k elements with exactly
 *            6 descents, and p_k_distinct(n) counts partitions of n into k
 *            distinct parts. The transformation n' = n - k*(k-1)/2 turns a
 *            distinct-part partition into a partition of n'...
 * Range:     u8 n <= 35, u16 n <= 48, u32 n <= 82, u64 n <= 204,
 *            u128 n <= 677, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

static seq_t *a_arr = NULL;   /* accumulated answer for each n */
static seq_t *p_prev = NULL;  /* p(n,k-1) values */
static seq_t *p_curr = NULL;  /* p(n,k)   values */
static seq_t *e_arr  = NULL;  /* Eulerian <k,6> for k up to current max */
static idx_t   max_n  = 0;
static idx_t   max_k  = 0;

/* Ensure all tables are computed for indices up to N. */
static void compute_to(idx_t N)
{
    if (N <= max_n) return;               /* nothing to do */

    /* new table size */
    idx_t new_n = N;
    /* estimate new maximum k such that k*(k+1)/2 <= N */
    idx_t new_k = 0;
    while ((new_k + 1) * (new_k + 2) / 2 <= new_n) new_k++;

    /* allocate new tables */
    seq_t *a_new   = (seq_t*)calloc(new_n + 1, sizeof(seq_t));
    seq_t *p1_new  = (seq_t*)calloc(new_n + 1, sizeof(seq_t));
    seq_t *p2_new  = (seq_t*)calloc(new_n + 1, sizeof(seq_t));
    seq_t *e_new   = (seq_t*)calloc(new_k + 1, sizeof(seq_t));

    /* ---------- Eulerian numbers <k,6> ---------- */
    seq_t e_prev[7] = {0}, e_cur[7] = {0};
    e_prev[0] = 1;                 /* <0,0> = 1 */
    e_new[0]  = 0;                 /* <0,6> = 0 */
    for (idx_t n = 1; n <= new_k; ++n) {
        for (int m = 0; m <= 6; ++m) e_cur[m] = 0;
        e_cur[0] = 1;                     /* <n,0> = 1 */
        for (int m = 1; m <= 6; ++m) {
            if ((idx_t)m <= n) {
                seq_t term1 = (n - (seq_t)m) * e_prev[m-1];
                seq_t term2 = (seq_t)(m + 1) * e_prev[m];
                e_cur[m] = term1 + term2;
            }
        }
        e_new[n] = e_cur[6];              /* store <n,6> */
        for (int m = 0; m <= 6; ++m) e_prev[m] = e_cur[m];
    }

    /* ---------- Partitions into at most k parts ---------- */
    /* initialize for k = 0 : only n = 0 has 1 partition */
    p1_new[0] = 1;          /* p_prev corresponds to k-1 (i.e., 0) */
    for (idx_t k = 1; k <= new_k; ++k) {
        /* compute p_curr[n] = p_prev[n] + p_curr[n-k] (for n>=k) */
        for (idx_t n = 0; n <= new_n; ++n) {
            seq_t val = p1_new[n];
            if (n >= k) val += p2_new[n - k];
            p2_new[n] = val;
        }

        /* contributions for this k (only if k >= 7) */
        seq_t e_k = e_new[k];
        if (e_k != 0) {
            idx_t min_n = k * (k + 1) / 2;
            if (min_n <= new_n) {
                for (idx_t n = min_n; n <= new_n; ++n) {
                    idx_t n_prime = n - k * (k - 1) / 2;
                    seq_t exact   = p2_new[n_prime];   /* p_atmost[n',k] */
                    exact -= p1_new[n_prime];          /* minus p_atmost[n',k-1] */
                    a_new[n] += exact * e_k;           /* accumulate */
                }
            }
        }

        /* swap p_prev and p_curr for next iteration; clear new curr */
        seq_t *tmp  = p1_new;
        p1_new      = p2_new;
        p2_new      = tmp;
        for (idx_t n = 0; n <= new_n; ++n) p2_new[n] = 0;
    }

    /* free old tables */
    if (a_arr)   free(a_arr);
    if (p_prev)  free(p_prev);
    if (p_curr)  free(p_curr);
    if (e_arr)   free(e_arr);

    /* adopt new tables */
    a_arr = a_new;
    p_prev = p1_new;
    p_curr = p2_new;
    e_arr  = e_new;
    max_n  = new_n;
    max_k  = new_k;
}

seq_t a(idx_t n)
{
    compute_to(n);
    return a_arr[n];
}
