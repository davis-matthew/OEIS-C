/*
 * A272631  Sum of three or more consecutive Fibonacci numbers.
 * https://oeis.org/A272631
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 46, u16 n <= 60, u32 n <= 60, u64 n <= 60, u128 n <= 60,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A272631: sums of at least three consecutive Fibonacci numbers,
   sorted increasingly, with the first term explicitly set to 2. */

#include <stdlib.h>
#include <stdbool.h>

/* comparison function for qsort */
static int cmp_seq_t(const void *p1, const void *p2)
{
    const seq_t *a = (const seq_t *)p1;
    const seq_t *b = (const seq_t *)p2;
    if (*a < *b) return -1;
    if (*a > *b) return 1;
    return 0;
}

seq_t a(idx_t n)
{
    static seq_t *seq = NULL;
    static size_t seq_len = 0;

    if (seq == NULL) {
        /* --- Build Fibonacci numbers that fit in seq_t --- */
        const idx_t MAX_FIB_IDX = 250;          /* generous upper bound  */
        seq_t *fib = (seq_t *)malloc((MAX_FIB_IDX + 3) * sizeof(seq_t));
        if (!fib) return 0;

        fib[1] = (seq_t)1;
        fib[2] = (seq_t)1;
        idx_t last = 2;

        for (idx_t i = 3; i <= MAX_FIB_IDX; ++i) {
            seq_t next = fib[i - 1] + fib[i - 2];
            /* Detect unsigned overflow (wrap‐around) or big‑int limit  */
            if (next < fib[i - 1]) {
                last = i - 1;
                break;
            }
            fib[i] = next;
            last = i;
        }

        /* --- Generate all sums of at least three consecutive Fibonacci numbers --- */
        size_t max_pairs = (last > 3) ? ((last - 2) * (last - 3) / 2) : 0;
        seq_t *candidates = (seq_t *)malloc(max_pairs * sizeof(seq_t));
        size_t cand_len = 0;
        if (!candidates) {
            free(fib);
            return 0;
        }

        for (idx_t aidx = 1; aidx <= last - 3; ++aidx) {
            for (idx_t bidx = aidx + 2; bidx <= last - 2; ++bidx) {
                /* sum F_a + ... + F_b = F_{b+2} – F_{a+1} */
                seq_t val = fib[bidx + 2] - fib[aidx + 1];
                /* simple guard; should not happen with given size estimates */
                if (cand_len >= max_pairs) {
                    size_t new_cap = max_pairs * 2;
                    seq_t *tmp = (seq_t *)realloc(candidates, new_cap * sizeof(seq_t));
                    if (!tmp) {
                        free(candidates);
                        free(fib);
                        return 0;
                    }
                    candidates = tmp;
                    max_pairs = new_cap;
                }
                candidates[cand_len++] = val;
            }
        }

        /* --- Sort and deduplicate candidate sums --- */
        qsort(candidates, cand_len, sizeof(seq_t), cmp_seq_t);

        /* Count unique values */
        size_t unique_len = 0;
        for (size_t i = 0; i < cand_len; ++i) {
            if (i == 0 || candidates[i] != candidates[i - 1]) {
                ++unique_len;
            }
        }

        /* Allocate final sequence array: add 1 for the initial 2 */
        seq = (seq_t *)malloc((unique_len + 1) * sizeof(seq_t));
        if (!seq) {
            free(candidates);
            free(fib);
            return 0;
        }

        seq[0] = (seq_t)2;                /* explicit first term */
        seq_len = unique_len + 1;
        size_t pos = 1;
        for (size_t i = 0; i < cand_len; ++i) {
            if (i == 0 || candidates[i] != candidates[i - 1]) {
                seq[pos++] = candidates[i];
            }
        }

        free(candidates);
        free(fib);
    }

    /* Return requested term if within bounds */
    if (n >= 1 && (idx_t)seq_len >= n) {
        return seq[n - 1];
    }
    return (seq_t)0;          /* undefined for out of range */
}
