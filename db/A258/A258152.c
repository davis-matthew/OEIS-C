/*
 * A258152  Partition array in Abramowitz-Stegun order for the number of ways
 *          of putting n stones into a rectangular m X n grid of squares such
 *          that each of the m rows contains at least one stone.
 * https://oeis.org/A258152
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy, tabf
 * Formula:   a(n) is the n‑th element of the flattened Abramowitz–Stegun
 *            partition triangle A258152. The value is computed by a(n,k) =
 *            (m! / Π_j e_j! ) * Π_j C(n,j)^{e_j} , where e_j is the
 *            multiplicity of part j in the partition of n with m parts. The
 *            sequence is defined for n ≥ 1; a(0) = 0 is returned
 * Range:     u8 n <= 9, u16 n <= 27, u32 n <= 44, u64 n <= 44, u128 n <= 44,
 *            big any n
 * Helpers:   binomial, factorial, seq_pow
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>
#include "../utils/factorial.h"
#include "../utils/binomial.h"
#include "../utils/seq_pow.h"

#define MAX_N 200   /* sufficient for flattened indices up to ≈10000 */

/* tables for ordinary partition numbers p(i) and their cumulative sums */
static idx_t part_cnt[MAX_N + 1];
static idx_t cum_cnt[MAX_N + 1];
static bool init_done = false;

/* initialise partition tables using DP with 128‑bit arithmetic */
static void init_partition_tables(void) {
    __int128 dp[MAX_N + 1] = {0};
    dp[0] = 1;
    for (int i = 1; i <= MAX_N; ++i)
        for (int j = i; j <= MAX_N; ++j)
            dp[j] += dp[j - i];

    cum_cnt[0] = 0;
    for (int i = 1; i <= MAX_N; ++i) {
        part_cnt[i] = (idx_t)dp[i];
        if (cum_cnt[i - 1] > (~(idx_t)0) - part_cnt[i])
            cum_cnt[i] = (~(idx_t)0); /* protect from overflow */
        else
            cum_cnt[i] = cum_cnt[i - 1] + part_cnt[i];
    }
    init_done = true;
}

/* compute a(n,k) for a given partition stored in parts[0..depth-1] */
static seq_t compute_value_for_parts(int *parts, int depth, int n) {
    int e[201] = {0};
    for (int i = 0; i < depth; ++i)
        ++e[parts[i]];

    seq_t coeff = factorial((idx_t)depth);
    for (int j = 1; j <= n; ++j)
        if (e[j] > 1)
            coeff = coeff / factorial((idx_t)e[j]);

    seq_t prod = 1;
    for (int j = 1; j <= n; ++j)
        if (e[j]) {
            seq_t bin = binomial((idx_t)n, (idx_t)j);
            prod = prod * seq_pow(bin, (idx_t)e[j]);
        }

    return coeff * prod;
}

/* recursive generator for partitions of 'remaining' into 'parts_left' parts
   with non‑increasing parts, each ≤ max_part.  Enumerates in the
   Abramowitz–Stegun order: first all 1‑part partitions, then 2‑part,
   etc., and within each fixed number of parts the parts are in
   lexicographic descending order.  When the target k is reached the
   result is stored in *res and *found is set true. */
static void gen_fixed_parts(idx_t remaining, int max_part, int parts_left,
                            int depth, int *parts, idx_t *k, seq_t *res,
                            int n, bool *found) {
    if (*found) return;
    if (parts_left == 0) {
        (*k)--;
        if (*k == 0) {
            *res = compute_value_for_parts(parts, depth, n);
            *found = true;
        }
        return;
    }
    if (remaining < (idx_t)parts_left) return;   /* insufficient sum */

    int p_max = max_part;
    int tmp = remaining - (parts_left - 1);
    if (tmp < p_max) p_max = (int)tmp;
    int p_min = (remaining + parts_left - 1) / parts_left;  /* ceil */
    for (int p = p_max; p >= p_min; --p) {
        parts[depth] = p;
        gen_fixed_parts(remaining - p, p, parts_left - 1,
                        depth + 1, parts, k, res, n, found);
        if (*found) return;
    }
}

/* main sequence function */
seq_t a(idx_t idx) {
    if (idx == 0) return (seq_t)0;
    if (!init_done) init_partition_tables();

    /* find the row n such that cum_cnt[n] ≥ idx */
    int row = 1;
    while (row <= MAX_N && cum_cnt[row] < idx)
        ++row;

    idx_t k = idx - cum_cnt[row - 1];         /* 1‑based offset within the row */
    int parts[201] = {0};
    bool found = false;
    seq_t result = 0;

    for (int m = 1; m <= row; ++m)
        gen_fixed_parts((idx_t)row, row, m, 0, parts,
                        &k, &result, row, &found);

    return result;
}
