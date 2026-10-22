/*
 * A302239  Expansion of Product_{k>=1} ((1 + x^k)/(1 - x^k))^p(k), where p(k)
 *          = number of partitions of k (A000041).
 * https://oeis.org/A302239
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 4, u16 n <= 10, u32 n <= 25, u64 n <= 33, u128 n <= 33,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A302239: a(n) = coeff of x^n in ∏_{k≥1} ((1+x^k)/(1−x^k))^{p(k)},
   where p(k) is the partition number A000041.
   Implementation uses the exponential series recurrence:
     n·g_n = Σ_{k=1..n} b_k · g_{n−k},  g_0 = 1,
   where b_k = 2·∑_{d|k, (k/d) odd} p(d)·d.  */
#include <stdlib.h>
#include <stdint.h>

/* static cache */
static idx_t  cache_size = 0;            /* max computed index + 1 */
static seq_t *part_arr   = NULL;         /* partition numbers */
static seq_t *b_arr      = NULL;         /* b_k coefficients */
static seq_t *g_arr      = NULL;         /* sequence values */

/* Allocate arrays up to n (inclusive), recompute all needed values. */
static void
recompute(idx_t n)
{
    size_t sz = (size_t)(n + 1);

    /* free old cache if any */
    if (part_arr) free(part_arr);
    if (b_arr)    free(b_arr);
    if (g_arr)    free(g_arr);

    part_arr = (seq_t *)malloc(sz * sizeof(seq_t));
    b_arr    = (seq_t *)malloc(sz * sizeof(seq_t));
    g_arr    = (seq_t *)malloc(sz * sizeof(seq_t));
    if (!part_arr || !b_arr || !g_arr)
        return;   /* in case of OOM – caller will return 0 */

    /* ----- compute partition numbers p[0..n] ----- */
    part_arr[0] = (seq_t)1;
    for (idx_t i = 1; i <= n; ++i) {
        part_arr[i] = (seq_t)0;
    }
    for (idx_t i = 1; i <= n; ++i) {
        for (idx_t j = i; j <= n; ++j) {
            part_arr[j] += part_arr[j - i];
        }
    }

    /* ----- compute b[k] for k = 1..n ----- */
    b_arr[0] = (seq_t)0;   /* unused */
    for (idx_t m = 1; m <= n; ++m) {
        seq_t sum = (seq_t)0;
        for (idx_t d = 1; d*d <= m; ++d) {
            if (m % d == 0) {
                idx_t e = m / d;          /* quotient */

                if ((e & 1) == 1)         /* e odd */
                    sum += (seq_t)2 * part_arr[d] * (seq_t)d;

                if (e != d) {
                    idx_t e2 = d;          /* second divisor */
                    if ((e2 & 1) == 1)     /* e2 odd */
                        sum += (seq_t)2 * part_arr[e] * (seq_t)e;
                }
            }
        }
        b_arr[m] = sum;
    }

    /* ----- compute sequence g[n] ----- */
    g_arr[0] = (seq_t)1;
    for (idx_t i = 1; i <= n; ++i) {
        seq_t sum = (seq_t)0;
        for (idx_t k = 1; k <= i; ++k) {
            sum += b_arr[k] * g_arr[i - k];
        }
        g_arr[i] = sum / (seq_t)i;   /* exact division */
    }

    cache_size = sz;
}

/* public function */
seq_t a(idx_t n)
{
    if (cache_size <= n)
        recompute(n);
    return (cache_size > n) ? g_arr[n] : (seq_t)0;
}
