/*
 * A322762  Irregular triangle read by rows: to get row n, take partitions of
 *          n ordered as in A080577, and in each partition, change each j-th
 *          occurrence of k to j; use compressed notation as in A322761.
 * https://oeis.org/A322762
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, tabf, look, base
 * Formula:   * For n ≥ 1, let r be the unique integer with * Σ_{i=1}^{r‑1}
 *            p(i) < n ≤ Σ_{i=1}^{r} p(i), * where p(i) is the number of
 *            partitions of i. * Let k = n – Σ_{i=1}^{r‑1} p(i). * The term
 *            a(n) is obtained by taking the k‑th partition of r in *
 *            non‑increasing order, replacing each j‑th occurrence of a...
 * Range:     u8 n <= 10, u16 n <= 28, u32 n <= 137, u64 n <= 682,
 *            u128 n <= 9294, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>

#define MAX_R 200   /* sufficient for n up to many thousands */

static bool part_counts_ready = false;
static idx_t part_counts[MAX_R + 1];
static idx_t cumulative_counts[MAX_R + 1];

/* Compute partition counts and cumulative sums up to MAX_R */
static void init_part_counts(void)
{
    if (part_counts_ready) return;
    for (idx_t i = 0; i <= MAX_R; ++i) part_counts[i] = 0;
    part_counts[0] = 1;
    for (idx_t part = 1; part <= MAX_R; ++part) {
        for (idx_t sum = part; sum <= MAX_R; ++sum) {
            part_counts[sum] += part_counts[sum - part];
        }
    }
    cumulative_counts[0] = 0;
    for (idx_t i = 1; i <= MAX_R; ++i) {
        cumulative_counts[i] = cumulative_counts[i - 1] + part_counts[i];
    }
    part_counts_ready = true;
}

/* Recursively locate the k‑th partition of 'remain' with parts ≤ max_part.
 * When the k‑th partition is found, *value is set to its compressed integer.
 */
static void find_kth_partition(idx_t remain, idx_t max_part,
                               int *parts, idx_t depth,
                               idx_t target, idx_t *count, seq_t *value)
{
    if (remain == 0) {
        (*count)++;
        if (*count == target) {
            seq_t val = 0;
            static int occ[MAX_R + 1];
            for (idx_t i = 1; i <= MAX_R; ++i) occ[i] = 0;
            for (idx_t i = 0; i < depth; ++i) {
                int p = parts[i];
                ++occ[p];
                idx_t j = occ[p];
                seq_t pow10 = 1;
                idx_t tmp = j;
                do {
                    pow10 *= 10;
                    tmp /= 10;
                } while (tmp);
                val = val * pow10 + (seq_t)j;
            }
            *value = val;
            (*count) = target + 1;   /* signal that we are done */
        }
        return;
    }
    for (idx_t p = (remain < max_part ? remain : max_part); p >= 1; --p) {
        parts[depth] = (int)p;
        find_kth_partition(remain - p, p, parts, depth + 1,
                           target, count, value);
        if (*count > target) return;   /* early exit after finding */
    }
}

seq_t a(idx_t n)
{
    if (n < 1) return (seq_t)0;  /* undefined for n < 1 */

    init_part_counts();

    /* Find the row r such that cumulative_counts[r-1] < n ≤ cumulative_counts[r] */
    idx_t r = 0;
    for (idx_t i = 1; i <= MAX_R; ++i) {
        if (cumulative_counts[i] >= n) {
            r = i;
            break;
        }
    }
    if (r == 0) return (seq_t)0;  /* safety, should not happen for n within bounds */

    idx_t prev_cum = cumulative_counts[r - 1];
    idx_t k = n - prev_cum;       /* 1-based index within row r */

    static int parts[MAX_R];
    idx_t count = 0;
    seq_t result = 0;
    find_kth_partition(r, r, parts, 0, k, &count, &result);
    return result;
}
