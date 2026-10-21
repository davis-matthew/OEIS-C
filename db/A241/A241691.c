/*
 * A241691  Number of Carlitz compositions of n with exactly one descent.
 * https://oeis.org/A241691
 *
 * Offset:    3    (a(3) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = number of Carlitz compositions of n with exactly one
 *            descent, no two adjacent parts equal. The computation below
 *            follows the combinatorial derived recurrence: a(n) =
 *            total_pairs(n) – descent_less(n) – equal_pairs(n) where
 *            total_pairs(n) = convolution of partition counts into distinct
 *            parts...
 * Range:     u8 n <= 14, u16 n <= 37, u32 n <= 115, u64 n <= 390,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdlib.h>

static seq_t *q_arr    = NULL;   /* partitions into distinct parts */
static seq_t *s_arr    = NULL;   /* sum of lengths across partitions */
static seq_t *conv_arr = NULL;   /* convolution of q */
static seq_t *perm_arr = NULL;   /* final sequence a(n) */
static seq_t *dp_all   = NULL;   /* subset sums of {1..n} */
static size_t       max_n = 0;    /* highest n computed */

static void precompute(idx_t up_to)
{
    if (up_to <= max_n) return;          /* already done */
    size_t N = (size_t)up_to;

    /* free previous arrays */
    free(q_arr);    free(s_arr);    free(conv_arr);
    free(perm_arr); free(dp_all);

    /* allocate new arrays */
    q_arr    = (seq_t*)calloc(N+1, sizeof(seq_t));
    s_arr    = (seq_t*)calloc(N+1, sizeof(seq_t));
    conv_arr = (seq_t*)calloc(N+1, sizeof(seq_t));
    perm_arr = (seq_t*)calloc(N+1, sizeof(seq_t));
    dp_all   = (seq_t*)calloc(N+1, sizeof(seq_t));

    /* -------- q[n] and s[n]  (partitions into distinct parts) -------- */
    q_arr[0] = 1;           /* empty partition */
    s_arr[0] = 0;
    for (size_t k = 1; k <= N; ++k) {
        for (size_t s = N; s >= k; --s) {          /* descending for distinct */
            q_arr[s] += q_arr[s - k];
            s_arr[s] += s_arr[s - k] + q_arr[s - k];
        }
    }

    /* -------- dp_all[s] : number of subsets of {1..N} summing to s -------- */
    dp_all[0] = 1;
    for (size_t val = 1; val <= N; ++val) {
        for (size_t s = N; s >= val; --s) {
            dp_all[s] += dp_all[s - val];
        }
    }

    /* -------- convolution: total_pairs[n] -------- */
    for (size_t m = 1; m < N; ++m) {
        seq_t qm = q_arr[m];
        for (size_t i = m + 1; i <= N; ++i) {
            conv_arr[i] += qm * q_arr[i - m];
        }
    }

    /* -------- descent_less[n] = s[n] - q[n] -------- */
    seq_t *descent_less = (seq_t*)malloc((N+1) * sizeof(seq_t));
    for (size_t n = 0; n <= N; ++n) {
        descent_less[n] = s_arr[n] - q_arr[n];
    }

    /* -------- equal_pairs[n] -------- */
    seq_t *equal_pairs = (seq_t*)calloc(N+1, sizeof(seq_t));
    seq_t *dp_excl = (seq_t*)calloc(N+1, sizeof(seq_t));   /* temporary */

    for (size_t t = 1; t <= N/2; ++t) {
        size_t maxS = N - 2*t;
        /* compute dp_excl[s] for s = 0..maxS */
        for (size_t s = 0; s <= maxS; ++s) {
            seq_t val = dp_all[s];
            if (s >= t) val -= dp_excl[s - t];
            dp_excl[s] = val;
        }
        /* accumulate contributions to equal_pairs[n] */
        for (size_t n = 2*t; n <= N; ++n) {
            equal_pairs[n] += dp_excl[n - 2*t];
        }
    }
    free(dp_excl);

    /* -------- final a[n] -------- */
    for (size_t n = 3; n <= N; ++n) {
        perm_arr[n] = conv_arr[n] - descent_less[n] - equal_pairs[n];
    }

    free(descent_less);
    free(equal_pairs);
    max_n = N;
}

seq_t a(idx_t n)
{
    if (n < 3) return (seq_t)0;  /* defined only for n>=3 */
    precompute(n);
    return perm_arr[(size_t)n];
}
