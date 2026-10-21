/*
 * A250505  Number of (n+2)X(1+2) 0..2 arrays with nondecreasing medians of
 *          every three consecutive values in every row and column.
 * https://oeis.org/A250505
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 none, u16 n <= 1, u32 n <= 5, u64 n <= 15, u128 n <= 35,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Algorithm: Count the 3×(n+2) tables of digits 0..2 whose 3‑element
 * sliding windows have nondecreasing medians, then cube the count
 * (the three columns are independent).  The DP stores, for a given
 * length L, the number of column sequences that satisfy the
 * nondecreasing‑median constraint. */
#include <stddef.h>
#include <stdbool.h>

/* Median of three digits 0..2. */
static inline int median3(int a, int b, int c)
{
    if (a > b) { int t = a; a = b; b = t; }
    if (b > c) { int t = b; b = c; c = t; }
    if (a > b) { int t = a; a = b; b = t; }
    return b;
}

/* Support up to n = 10000 as required. */
#define MAXN 10000
#define MAXL (MAXN + 2)   /* maximal sequence length L = n+2 */

/* Cache of total counts for each sequence length (3..MAXL). */
static seq_t count_cache[MAXL + 1] = {0};

/* Current DP state for the last computed length. */
static seq_t dp_cur[27] = {0};
static seq_t dp_next[27] = {0};
static idx_t cur_len = 0;      /* length for which dp_cur is valid */
static bool initialized = false;

/* Main function required. */
seq_t a(idx_t n)
{
    idx_t L = n + 2;            /* required sequence length */

    /* Initialize DP for length 3 on first call. */
    if (!initialized) {
        for (int i = 0; i < 27; ++i) dp_cur[i] = 0;

        for (int x = 0; x < 3; ++x)
            for (int y = 0; y < 3; ++y)
                for (int z = 0; z < 3; ++z) {
                    int m = median3(x, y, z);
                    int idx = ((y * 3 + z) * 3 + m);
                    dp_cur[idx] += (seq_t)1;
                }

        seq_t total = 0;
        for (int i = 0; i < 27; ++i) total += dp_cur[i];
        count_cache[3] = total;
        cur_len = 3;
        initialized = true;
    }

    /* Extend the DP to the length required by L. */
    while (cur_len < L) {
        for (int i = 0; i < 27; ++i) dp_next[i] = 0;

        for (int idx = 0; idx < 27; ++idx) {
            seq_t cnt = dp_cur[idx];
            if (!cnt) continue;

            int m = idx % 3;
            int t = idx / 3;
            int z = t % 3;
            int y = t / 3;

            for (int w = 0; w < 3; ++w) {
                int new_m = median3(y, z, w);
                if (m <= new_m) {
                    int new_idx = ((z * 3 + w) * 3 + new_m);
                    dp_next[new_idx] += cnt;
                }
            }
        }

        for (int i = 0; i < 27; ++i) dp_cur[i] = dp_next[i];

        seq_t total = 0;
        for (int i = 0; i < 27; ++i) total += dp_cur[i];
        ++cur_len;
        count_cache[cur_len] = total;
    }

    seq_t cnt = count_cache[L];
    /* Three independent columns ➜ cube the count. */
    return cnt * cnt * cnt;
}
