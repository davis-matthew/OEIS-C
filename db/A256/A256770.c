/*
 * A256770  Number of (n+2)X(7+2) 0..1 arrays with no 3x3 subblock diagonal
 *          sum less than the antidiagonal sum.
 * https://oeis.org/A256770
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = number of (n+2)x9 binary arrays with no 3x3 subblock *
 *            diagonal sum < antidiagonal sum, computed via DP over parity
 *            classes
 * Range:     u8 none, u16 none, u32 n <= 4, u64 n <= 21, u128 n <= 31,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

/* Dimensions for the difference vectors. */
#define DIFF_DIM 7
#define N_DIFF 2187  /* 3^7 */

#define MAX_N 20000   /* safe upper bound for n for the array size */
#define MAX_L ((MAX_N + 3) / 2 + 2)  /* maximum possible L_even or L_odd */

/* Pre‑computed powers of 3. */
static int pow3[DIFF_DIM] = {0};

/* Tables for the DP. */
static seq_t w_counts[N_DIFF];
static seq_t dp_even_cur_arr[N_DIFF], dp_even_next_arr[N_DIFF];
static seq_t dp_odd_cur_arr[N_DIFF], dp_odd_next_arr[N_DIFF];
static seq_t sum_sup_arr[N_DIFF];

static seq_t *dp_even_cur = dp_even_cur_arr;
static seq_t *dp_even_next = dp_even_next_arr;
static seq_t *dp_odd_cur = dp_odd_cur_arr;
static seq_t *dp_odd_next = dp_odd_next_arr;

/* Result caches for each parity class. */
static seq_t results_even[MAX_L];
static seq_t results_odd[MAX_L];
static size_t even_cap = 1;
static size_t odd_cap = 1;

/* Initialise global tables on first use. */
static void init_global(void)
{
    static bool inited = false;
    if (inited) return;
    inited = true;

    /* powers of 3 */
    pow3[0] = 1;
    for (int d = 1; d < DIFF_DIM; ++d) pow3[d] = pow3[d - 1] * 3;

    /* count how many 9‑bit rows produce each difference vector */
    memset(w_counts, 0, sizeof(w_counts));
    for (int row = 0; row < 512; ++row)
    {
        int code = 0;
        for (int j = 0; j < DIFF_DIM; ++j)
        {
            int a = (row >> j) & 1;
            int b = (row >> (j + 2)) & 1;
            int diff = a - b;          /* -1,0,1 */
            int digit = diff + 1;       /* 0,1,2 */
            code += digit * pow3[j];
        }
        w_counts[code] = w_counts[code] + 1;
    }

    /* initialise DP for length 1 for both parities */
    for (int i = 0; i < N_DIFF; ++i)
    {
        dp_even_cur_arr[i] = w_counts[i];
        dp_odd_cur_arr[i]  = w_counts[i];
    }

    /* total for length 1 */
    seq_t total = 0;
    for (int i = 0; i < N_DIFF; ++i) total += w_counts[i];
    results_even[1] = total;
    results_odd[1]  = total;
    even_cap = odd_cap = 1;
}

/* Superset sum over the componentwise order (octahedral). */
static void compute_superset_sums(const seq_t *cur, seq_t *sums)
{
    memcpy(sums, cur, N_DIFF * sizeof(seq_t));
    for (int d = 0; d < DIFF_DIM; ++d)
    {
        int step = pow3[d];
        for (int idx = 0; idx < N_DIFF; ++idx)
        {
            int digit = (idx / step) % 3;
            if (digit <= 1) sums[idx] += sums[idx + step];
            if (digit == 0) sums[idx] += sums[idx + 2 * step];
        }
    }
}

/* Ensure results_even is computed up to needed length. */
static void ensure_even_len(size_t needed)
{
    while (even_cap < needed)
    {
        compute_superset_sums(dp_even_cur, sum_sup_arr);

        seq_t total = 0;
        for (int j = 0; j < N_DIFF; ++j)
        {
            dp_even_next[j] = sum_sup_arr[j] * w_counts[j];
            total += dp_even_next[j];
        }

        /* swap buffers */
        seq_t *tmp = dp_even_cur;
        dp_even_cur = dp_even_next;
        dp_even_next = tmp;

        ++even_cap;
        results_even[even_cap] = total;
    }
}

/* Ensure results_odd is computed up to needed length. */
static void ensure_odd_len(size_t needed)
{
    while (odd_cap < needed)
    {
        compute_superset_sums(dp_odd_cur, sum_sup_arr);

        seq_t total = 0;
        for (int j = 0; j < N_DIFF; ++j)
        {
            dp_odd_next[j] = sum_sup_arr[j] * w_counts[j];
            total += dp_odd_next[j];
        }

        /* swap buffers */
        seq_t *tmp = dp_odd_cur;
        dp_odd_cur = dp_odd_next;
        dp_odd_next = tmp;

        ++odd_cap;
        results_odd[odd_cap] = total;
    }
}

/* Public function */
seq_t a(idx_t n)
{
    init_global();

    size_t N_rows = (size_t)n + 2;                 /* total rows */
    size_t L_even = (N_rows + 1) / 2;              /* ceil(N_rows/2) */
    size_t L_odd  = N_rows / 2;                    /* floor(N_rows/2) */

    if (even_cap < L_even) ensure_even_len(L_even);
    if (odd_cap  < L_odd ) ensure_odd_len (L_odd);

    return results_even[L_even] * results_odd[L_odd];
}
