/*
 * A308931  Sum of the third largest parts in the partitions of n into 7
 *          parts.
 * https://oeis.org/A308931
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 20, u16 n <= 49, u32 n <= 49, u64 n <= 49, u128 n <= 49,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A308931: sum of the third largest parts in the partitions
   of n into exactly 7 parts.
   Algorithm: a DP over the weighted equation
          n = 7 + 7*t₁ + 6*t₂ + 5*t₃ + 4*t₄ + 3*t₅ + 2*t₆ + 1*t₇ ,
   where tᵢ ≥ 0.  The third largest part equals 1 + t₁ + t₂ + t₃ + t₄ + t₅.
   For each reachable weighted sum S = n‑7 we keep
        cnt[S] – number of solutions,
        sum[S] – total of (t₁+…+t₅) over those solutions.
   The DP updates these arrays with the seven coin weights
   {7,6,5,4,3,2,1}.  After processing all weights
   a(n) = cnt[S] + sum[S].
   Complexity:  O(n · 7) time and O(n) memory. */

#include <stdlib.h>
#include <stdint.h>

static const idx_t weights[7] = {7, 6, 5, 4, 3, 2, 1};

/* cached DP arrays */
static seq_t *cnt_arr = NULL;
static seq_t *sum_arr = NULL;
static idx_t  cached_cap = 0;

/* Ensure the DP arrays can hold indices up to max_size.  */
static void
ensure_arrays(idx_t max_size)
{
    if (cached_cap > max_size)
        return;
    /* Allocate new larger arrays. */
    seq_t *new_cnt = (seq_t *)malloc((max_size + 1) * sizeof(seq_t));
    seq_t *new_sum = (seq_t *)malloc((max_size + 1) * sizeof(seq_t));
    if (!new_cnt || !new_sum)
        abort();                  /* allocation failure */

    /* Free old arrays if they exist. */
    if (cnt_arr) free(cnt_arr);
    if (sum_arr) free(sum_arr);

    cnt_arr = new_cnt;
    sum_arr = new_sum;
    cached_cap = max_size + 1;
}

seq_t a(idx_t n)
{
    if (n < 7)
        return (seq_t)0;          /* no partition of 7 parts into smaller sum */

    idx_t S = n - 7;              /* weighted sum to be matched */
    ensure_arrays(S);

    /* Reset the DP tables up to S. */
    for (idx_t i = 0; i <= S; ++i) {
        cnt_arr[i] = (seq_t)0;
        sum_arr[i] = (seq_t)0;
    }
    cnt_arr[0] = (seq_t)1;        /* empty solution */

    /* DP over the seven difference variables. */
    for (int k = 0; k < 7; ++k) {
        idx_t w = weights[k];
        int  add_to_sum = (k <= 4);   /* t₁ … t₅ contribute to the third part */

        for (idx_t s = w; s <= S; ++s) {
            cnt_arr[s] += cnt_arr[s - w];
            if (add_to_sum)
                sum_arr[s] += sum_arr[s - w] + cnt_arr[s - w];
            else
                sum_arr[s] += sum_arr[s - w];
        }
    }

    return cnt_arr[S] + sum_arr[S];
}
