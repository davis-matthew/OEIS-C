/*
 * A328027  Irregular triangle read by rows where row n lists, in weakly
 *          increasing order, the differences between consecutive divisors of
 *          n.
 * https://oeis.org/A328027
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, tabf
 * Formula:   not stated
 * Range:     u8 n <= 81, u16 n <= 81, u32 n <= 81, u64 n <= 81, u128 n <= 81,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Compute sequence A328027: for each integer m≥2, take the divisors of m
   in increasing order, compute the differences between consecutive
   divisors, sort those differences in weakly increasing order, and
   append them to the flattened sequence.  The function returns the
   n‑th element of that flattened list (1‑based offset). */
#include <stdlib.h>
#include <stddef.h>

static seq_t *data   = NULL;   /* flattened sequence            */
static size_t data_size = 0;    /* number of terms stored       */
static size_t data_cap   = 0;    /* capacity of data buffer      */
static idx_t   last_m    = 1;    /* largest m having been processed */

/* Grow the data buffer to hold at least `needed` elements. */
static void grow_seq(size_t needed)
{
    if (needed <= data_cap)
        return;
    size_t new_cap = data_cap ? data_cap * 2 : 16;
    while (new_cap < needed)
        new_cap <<= 1;
    seq_t *new_ptr = (seq_t *)realloc(data, new_cap * sizeof(seq_t));
    if (!new_ptr)
        exit(EXIT_FAILURE);
    data = new_ptr;
    data_cap = new_cap;
}

/* Main entry point.  The function is called with n ≥ 1. */
seq_t a(idx_t n)
{
    if (n == 0)          /* offset is 1, but keep safe for 0   */
        return 0;

    /* Generate rows until we have at least n terms. */
    while (data_size < (size_t)n) {
        idx_t m = last_m + 1;   /* next integer to process */

        /* Collect small (ascending) and big (descending) divisors. */
        idx_t small[256], big[256];
        size_t s_cnt = 0, b_cnt = 0;
        for (idx_t i = 1; i <= m / i; ++i) {
            if (m % i == 0) {
                small[s_cnt++] = i;
                if (i != m / i)
                    big[b_cnt++] = m / i;   /* stored in reverse order */
            }
        }

        /* Total differences = total divisors – 1. */
        size_t diff_count = (s_cnt + b_cnt > 0) ? (s_cnt + b_cnt - 1) : 0;
        if (diff_count == 0) {
            last_m = m;
            continue;   /* m has only one divisor (m==1, but we never reach it) */
        }

        /* Compute all differences (in any order). */
        seq_t diffs[256];
        size_t di = 0;

        for (size_t i = 1; i < s_cnt; ++i)
            diffs[di++] = (seq_t)(small[i] - small[i - 1]);

        /* Differences from the largest small divisor to the big ones. */
        if (b_cnt > 0) {
            idx_t prev = small[s_cnt - 1];
            for (size_t k = b_cnt; k-- > 0; ) {
                idx_t cur = big[k];
                diffs[di++] = (seq_t)(cur - prev);
                prev = cur;
            }
        }

        /* Sort the differences in weakly increasing order. */
        for (size_t i = 1; i < diff_count; ++i) {
            seq_t val = diffs[i];
            size_t j = i;
            while (j > 0 && diffs[j - 1] > val) {
                diffs[j] = diffs[j - 1];
                --j;
            }
            diffs[j] = val;
        }

        /* Append sorted differences to the global buffer. */
        grow_seq(data_size + diff_count);
        for (size_t i = 0; i < diff_count; ++i)
            data[data_size++] = diffs[i];

        last_m = m;
    }

    return data[n - 1];
}
