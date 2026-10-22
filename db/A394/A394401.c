/*
 * A394401  A sequence constructed by greedily sampling the probability
 *          distribution given by 1/log_2(i+1) - 1/log_2(i+2) to minimize
 *          discrepancy.
 * https://oeis.org/A394401
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 1297, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   p_of_k
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A394401 –  greedy discrepancy minimisation
   of the distribution p(k) = 1/log2(k+1) - 1/log2(k+2)  */
#include <stdlib.h>
#include <math.h>
#include "../utils/p_of_k.h"

/* static buffers for memoisation */
static seq_t *seq = NULL;          /* computed sequence values */
static size_t seq_cap = 0;         /* capacity of seq */
static idx_t seq_len = 0;          /* number of terms already known */

static idx_t *cnt = NULL;          /* occurrence counts of each k */
static size_t cnt_cap = 0;         /* capacity of cnt */
static idx_t cnt_len = 0;          /* highest k stored + 1 */

/* enlarge seq buffer if required */
static void ensure_seq_cap(idx_t needed)
{
    if ((size_t)needed > seq_cap) {
        size_t new_cap = seq_cap ? seq_cap : 8;
        while (new_cap < (size_t)needed) new_cap <<= 1;
        seq = (seq_t *)realloc(seq, new_cap * sizeof(seq_t));
        seq_cap = new_cap;
    }
}

/* enlarge cnt buffer if required */
static void ensure_cnt_cap(idx_t k)
{
    if ((size_t)k >= cnt_cap) {
        size_t new_cap = cnt_cap ? cnt_cap : 8;
        while (new_cap <= (size_t)k) new_cap <<= 1;
        cnt = (idx_t *)realloc(cnt, new_cap * sizeof(idx_t));
        for (size_t i = cnt_len; i < new_cap; ++i) cnt[i] = 0;
        cnt_cap = new_cap;
    }
}

/* compute a(n) – OEIS A394401 */
seq_t a(idx_t n)
{
    if (n < 1) return 0;          /* offset starts at 1 */

    /* generate terms up to n if not already done */
    while (seq_len < n) {
        idx_t m = seq_len + 1;    /* current index for which we compute the term */

        /* initialise counting array to hold at least k = 1 */
        if (cnt_cap < 2) ensure_cnt_cap(1);

        /* find k that maximises n*p(k) - cnt[k] */
        idx_t best_k = 1;
        double best_diff = (double)m * p_of_k(1) - (double)cnt[1];

        for (idx_t k = 2;; ++k) {
            double exp_k = (double)m * p_of_k(k);
            /* once exp_k <= best_diff further k cannot beat the current best */
            if (exp_k <= best_diff + 1e-12) break;

            double diff = exp_k - (double)(k < cnt_len ? cnt[k] : 0);
            if (diff > best_diff + 1e-12) {
                best_diff = diff;
                best_k    = k;
            }
        }

        /* store the chosen term */
        ensure_seq_cap(seq_len + 1);
        seq[seq_len] = (seq_t)best_k;
        seq_len++;

        /* update occurrence counter for the chosen k */
        ensure_cnt_cap(best_k);
        cnt[best_k]++;
        if ((idx_t)best_k >= cnt_len) cnt_len = best_k + 1;
    }

    return seq[n - 1];
}
