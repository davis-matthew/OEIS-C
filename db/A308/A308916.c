/*
 * A308916  Irregular triangular array: row n shows positions of strict
 *          partitions of n among all partitions of n, using Mathematica
 *          ordering.
 * https://oeis.org/A308916
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, tabf, easy
 * Formula:   not stated
 * Range:     u8 n <= 79, u16 n <= 79, u32 n <= 79, u64 n <= 79, u128 n <= 79,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computation: generates the position of each strict partition among all
 * partitions of n, sorted in Mathematica (lexicographic) order.
 * The sequence a(n) is formed by flattening these rows row‑by‑row. */
#include <stdlib.h>

/* -------------- Global state -------------- */
static seq_t   *values     = NULL;  /* flattened array of positions   */
static size_t  values_len = 0;      /* current number of stored terms  */
static size_t  values_cap = 0;      /* allocated capacity of values    */
static int     current_row = 0;     /* rows generated so far           */

/* -------------- DP table: partitions with bounded part -------------- */
static seq_t *dp = NULL;   /* dp[m][j] = #partitions of m using parts <= j   */
static int   dp_n = 0;     /* current maximum n for which dp is defined     */

/* -------------- Helpers for dynamic arrays -------------- */
static void ensure_values_cap(size_t needed)
{
    if (needed <= values_cap) return;
    size_t new_cap = values_cap ? values_cap * 2 : 8;
    if (new_cap < needed) new_cap = needed;
    seq_t *tmp = (seq_t*)realloc(values, new_cap * sizeof(seq_t));
    if (!tmp) exit(1);
    values = tmp;
    values_cap = new_cap;
}

/* -------------- Recompute DP table up to n (inclusive) -------------- */
static void recompute_dp(int n)
{
    if (n <= dp_n) return;            /* already computed up to this n   */
    size_t size  = (size_t)(n + 1) * (n + 1);
    seq_t *new_dp = (seq_t*)malloc(size * sizeof(seq_t));
    if (!new_dp) exit(1);

    int width = n + 1;
    for (int m = 0; m <= n; ++m) {
        new_dp[m * width + 0] = (m == 0 ? 1 : 0);   /* base: empty partition  */
        for (int j = 1; j <= n; ++j) {
            seq_t val = new_dp[m * width + (j - 1)];
            if (j <= m) {
                val += new_dp[(m - j) * width + j];
            }
            new_dp[m * width + j] = val;
        }
    }
    if (dp) free(dp);
    dp   = new_dp;
    dp_n = n;
}

/* -------------- Rank of a strict partition -------------- */
static seq_t rank_of_partition(const int *parts, int len, int n)
{
    seq_t rank   = 1;                 /* positions start from 1       */
    int   prefix = 0;
    int   max_next = n;
    int   width   = dp_n + 1;         /* dp table width                */

    for (int i = 0; i < len; ++i) {
        int curr = parts[i];
        for (int t = max_next; t > curr; --t) {
            int rest = n - prefix - t;
            if (rest >= 0) {
                /* dp[rest][t] is the number of partitions of rest
                 * using parts <= t, i.e. the number of completions to
                 * a partition whose prefix matches the first i parts
                 * and whose next part is exactly t.                 */
                rank += dp[rest * width + t];
            }
        }
        prefix  += curr;
        max_next = curr;
    }
    return rank;
}

/* -------------- Recursive generation of distinct partitions -------------- */
static void gen_parts_recursive(int rem, int max_allowed, int prev,
                                int depth, int *parts, int n)
{
    if (rem == 0) {                     /* strict partition completed  */
        seq_t r = rank_of_partition(parts, depth, n);
        ensure_values_cap(values_len + 1);
        values[values_len++] = r;
        return;
    }
    int upper = rem < max_allowed ? rem : max_allowed;
    for (int part = upper; part >= 1; --part) {
        if (part >= prev) continue;     /* enforce distinctness        */
        parts[depth] = part;
        int next_max = part - 1;
        if (next_max > rem - part) next_max = rem - part;
        gen_parts_recursive(rem - part, next_max, part,
                            depth + 1, parts, n);
    }
}

/* -------------- Generate one whole row -------------- */
static void generate_row(int row)
{
    recompute_dp(row);                  /* ensure DP table covers this n */
    int parts[2000];                    /* works for any row up to 2000  */
    gen_parts_recursive(row, row, row + 1, 0, parts, row);
}

/* -------------- Main OEIS function -------------- */
seq_t a(idx_t n)
{
    if (n == 0) return 0;               /* offset starts at 1            */
    while (values_len < (size_t)n) {     /* generate more rows if needed   */
        ++current_row;
        generate_row(current_row);
    }
    return values[n - 1];
}
