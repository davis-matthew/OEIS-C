/*
 * A358915  a(n) is the far-difference representation of n written in balanced
 *          ternary.
 * https://oeis.org/A358915
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 n <= 15, u16 n <= 49, u32 n <= 49, u64 n <= 49, u128 n <= 49,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* This implementation builds the A358915 sequence by enumerating all
 * valid far‑difference representations up to an increasing maximum
 * Fibonacci index until the requested index n is present.
 * A representation is a set of signed indices (±i) satisfying the
 * spacing rules; the resulting value is the sum of 3^i with the
 * appropriate sign.  The function generates the values in sorted
 * order, removes duplicates, and caches the results for later calls. */

#include <stdlib.h>
#include <stdint.h>

/* Forward declarations of helper functions */
static int compare_seq_t(const void *a, const void *b);
static void dfs_rec(int pos, int last_plus, int last_minus,
                    seq_t cur_pos_sum, seq_t cur_neg_sum,
                    int max_idx, seq_t *pow3,
                    seq_t **buf, size_t *count, size_t *cap);
static void generate_all(int max_idx, seq_t **out, size_t *cnt);

/* Comparator for qsort */
static int compare_seq_t(const void *a, const void *b)
{
    seq_t x = *(const seq_t *)a;
    seq_t y = *(const seq_t *)b;
    return (x < y) ? -1 : (x > y);
}

/* Recursive depth‑first search that builds all valid representations.
 * `pos`   : next index to consider
 * `last_plus` / `last_minus` : index of the last positive/negative term
 * `cur_pos_sum` / `cur_neg_sum` : cumulative sums of positive/negative terms
 */
static void dfs_rec(int pos, int last_plus, int last_minus,
                    seq_t cur_pos_sum, seq_t cur_neg_sum,
                    int max_idx, seq_t *pow3,
                    seq_t **buf, size_t *count, size_t *cap)
{
    /* Store the current value if non‑negative */
    if (cur_pos_sum >= cur_neg_sum) {
        if (*count >= *cap) {
            *cap *= 2;
            *buf = (seq_t *)realloc(*buf, *cap * sizeof(seq_t));
        }
        (*buf)[*count] = cur_pos_sum - cur_neg_sum;
        (*count)++;
    }

    /* Try to place next terms */
    for (int i = pos; i <= max_idx; i++) {
        /* Add a + term at position i */
        if ((last_plus < 0 || i - last_plus >= 4) &&
            (last_minus < 0 || i - last_minus >= 3)) {
            dfs_rec(i + 1, i, last_minus,
                    cur_pos_sum + pow3[i], cur_neg_sum,
                    max_idx, pow3, buf, count, cap);
        }
        /* Add a - term at position i */
        if ((last_minus < 0 || i - last_minus >= 4) &&
            (last_plus < 0 || i - last_plus >= 3)) {
            dfs_rec(i + 1, last_plus, i,
                    cur_pos_sum, cur_neg_sum + pow3[i],
                    max_idx, pow3, buf, count, cap);
        }
    }
}

/* Generates all valid values using indices up to `max_idx` and returns
 * the sorted unique list in *out with the number of elements in *cnt. */
static void generate_all(int max_idx, seq_t **out, size_t *cnt)
{
    /* Precompute powers of 3 */
    seq_t *pow3 = (seq_t *)malloc((max_idx + 1) * sizeof(seq_t));
    pow3[0] = (seq_t)1;
    for (int i = 1; i <= max_idx; i++)
        pow3[i] = pow3[i - 1] * (seq_t)3;

    size_t cap = 128;
    seq_t *buf = (seq_t *)malloc(cap * sizeof(seq_t));
    size_t count = 0;

    dfs_rec(0, -1, -1, (seq_t)0, (seq_t)0, max_idx, pow3,
            &buf, &count, &cap);

    free(pow3);

    /* Sort and deduplicate */
    qsort(buf, count, sizeof(seq_t), compare_seq_t);
    size_t uniq = 0;
    for (size_t i = 0; i < count; i++) {
        if (uniq == 0 || buf[i] != buf[uniq - 1])
            buf[uniq++] = buf[i];
    }
    *out = buf;
    *cnt = uniq;
}

/* --------------------------------------------------------------------- */
/* Public sequence function */
seq_t a(idx_t n)
{
    /* Static cache of generated values */
    static seq_t *cache = NULL;
    static size_t cache_cnt = 0;
    static int max_idx_global = -1;

    if ((idx_t)n >= cache_cnt) {
        int new_idx = (max_idx_global >= 0) ? max_idx_global + 5 : 5;
        for (;;) {
            seq_t *tmp;
            size_t tmpcnt;
            generate_all(new_idx, &tmp, &tmpcnt);

            if ((idx_t)n < (idx_t)tmpcnt) {
                /* Replace cache with the larger set */
                free(cache);
                cache = tmp;
                cache_cnt = tmpcnt;
                max_idx_global = new_idx;
                break;
            }
            free(tmp);
            new_idx += 5;   /* increase search window */
        }
    }
    return cache[(size_t)n];
}
