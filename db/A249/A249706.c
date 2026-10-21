/*
 * A249706  Number of length n+3 0..7 arrays with every four consecutive terms
 *          having the maximum of some two terms equal to the minimum of the
 *          remaining two terms.
 * https://oeis.org/A249706
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = number of length n+3 arrays over {0,…,7} with the
 *            property that in every consecutive block of four terms the
 *            maximum of some two equals the minimum of the remaining two.
 *            Computed via dynamic programming on the last three values
 *            (states of size 8³=512)
 * Range:     u8 none, u16 n <= 5, u32 n <= 16, u64 n <= 37, u128 n <= 81,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include <stdlib.h>

static bool valid[8][8][8][8] = {0};

static void compute_valid(void)
{
    for (int a = 0; a < 8; ++a)
        for (int b = 0; b < 8; ++b)
            for (int c = 0; c < 8; ++c)
                for (int d = 0; d < 8; ++d) {
                    bool ok = false;
                    int arr[4] = {a, b, c, d};
                    for (int i = 0; i < 4 && !ok; ++i)
                        for (int j = i + 1; j < 4 && !ok; ++j) {
                            int max1 = (arr[i] > arr[j]) ? arr[i] : arr[j];
                            int others[2], t = 0;
                            for (int k = 0; k < 4; ++k)
                                if (k != i && k != j)
                                    others[t++] = arr[k];
                            int min2 = (others[0] < others[1]) ? others[0] : others[1];
                            if (max1 == min2)
                                ok = true;
                        }
                    valid[a][b][c][d] = ok;
                }
}

static seq_t dp1[512], dp2[512];
static seq_t *dp_cur, *dp_next;
static seq_t *results = NULL;
static size_t results_size = 0;   /* results[0] unused, results[1] = a(1) */
static bool initialized = false;

static void init_static(void)
{
    if (initialized) return;
    compute_valid();

    dp_cur = dp1;
    dp_next = dp2;
    for (int i = 0; i < 512; ++i) dp_cur[i] = 0;

    /* initial sequences of length 4 (a(1)) */
    for (int a = 0; a < 8; ++a)
        for (int b = 0; b < 8; ++b)
            for (int c = 0; c < 8; ++c)
                for (int d = 0; d < 8; ++d)
                    if (valid[a][b][c][d]) {
                        int idx = (b << 6) | (c << 3) | d;
                        dp_cur[idx] += (seq_t)1;
                    }

    seq_t sum = 0;
    for (int i = 0; i < 512; ++i) sum += dp_cur[i];

    results_size = 2;                      /* need indices 0 and 1 */
    results = (seq_t *)calloc(results_size, sizeof(seq_t));
    results[1] = sum;                      /* a(1) */

    initialized = true;
}

static void compute_up_to(idx_t target_n)
{
    init_static();
    while (results_size - 1 < target_n) {
        /* zero dp_next */
        for (int i = 0; i < 512; ++i) dp_next[i] = 0;

        /* transition from current length to next */
        for (int idx = 0; idx < 512; ++idx) {
            seq_t c = dp_cur[idx];
            if (c == 0) continue;
            int x = idx >> 6;
            int y = (idx >> 3) & 7;
            int z = idx & 7;
            for (int w = 0; w < 8; ++w)
                if (valid[x][y][z][w]) {
                    int next_idx = (y << 6) | (z << 3) | w;
                    dp_next[next_idx] += c;
                }
        }

        /* sum for the new n */
        seq_t sum = 0;
        for (int i = 0; i < 512; ++i) sum += dp_next[i];

        results = (seq_t *)realloc(results,
                                   (results_size + 1) * sizeof(seq_t));
        results[results_size] = sum;   /* store a(new n) */

        /* prepare for next iteration */
        seq_t *tmp = dp_cur;
        dp_cur = dp_next;
        dp_next = tmp;
        ++results_size;
    }
}

seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0;
    compute_up_to(n);
    return results[n];
}
