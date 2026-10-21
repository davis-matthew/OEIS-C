/*
 * A252160  Number of (n+2)X(1+2) 0..3 arrays with every 3X3 subblock row and
 *          column sum equal to 2 3 4 6 or 7 and every 3X3 diagonal and
 *          antidiagonal sum not equal to 2 3 4 6 or 7.
 * https://oeis.org/A252160
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 none, u16 n <= 7, u32 n <= 24, u64 n <= 57, u128 n <= 122,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*
 *  OEIS sequence A252160 – count of (n+2)×3 arrays over {0,1,2,3}
 *  such that every 3×3 subblock has each row and column sum in
 *  {2,3,4,6,7} and each 3×3 diagonal and antidiagonal sum NOT in that set.
 *
 *  The algorithm builds a directed graph whose nodes are pairs of
 *  consecutive rows (a,b) and edges represent a possible next row c.
 *  A dynamic‑programming walk over this graph counts all admissible
 *  arrays.  The first, 3‑row arrays correspond to the graph edges;
 *  afterwards each additional row multiplies the number of walks.
 *  Results are memoised so that arbitrary calls to a(n) are answered in
 *  O(1) after the graph is built.
 */

#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <stddef.h>

/* ----------  Global state  ------------------------------------------- */
static bool initialized = false;   /* do we have the transition graph? */
static idx_t current_n = 0;        /* largest n cached (1‑based) */
static seq_t *cur = NULL;          /* count per state for current height */
static seq_t *next_arr = NULL;     /* buffer during transition */
static int *offset = NULL;         /* start index of edges for each state */
static int *edges_pool = NULL;     /* all edges concatenated */
static seq_t *cache = NULL;        /* cached a(n) values, cache[0] = a(1) */
static size_t cache_len = 0, cache_cap = 0;

/* ----------  Build the transition graph  ---------------------------- */
static void init_tables(void)
{
    /* Pre‑compute digits of each 3‑row (0..63) and its row sum */
    static int row_digits[64][3];
    static int row_sum[64];
    static bool allowed[10] = {0};

    allowed[2] = allowed[3] = allowed[4] = allowed[6] = allowed[7] = true;

    for (int r = 0; r < 64; ++r) {
        row_digits[r][0] = r & 3;
        row_digits[r][1] = (r >> 2) & 3;
        row_digits[r][2] = (r >> 4) & 3;
        row_sum[r] = row_digits[r][0] + row_digits[r][1] + row_digits[r][2];
    }

    /* Count outgoing edges per state, and build edge list */
    int edge_counts[4096];
    memset(edge_counts, 0, sizeof(edge_counts));

    for (int a = 0; a < 64; ++a) {
        if (!allowed[row_sum[a]]) continue;
        for (int b = 0; b < 64; ++b) {
            if (!allowed[row_sum[b]]) continue;
            for (int c = 0; c < 64; ++c) {
                if (!allowed[row_sum[c]]) continue;

                /* 3×3 column sums */
                if (!allowed[row_digits[a][0] + row_digits[b][0] + row_digits[c][0]])
                    continue;
                if (!allowed[row_digits[a][1] + row_digits[b][1] + row_digits[c][1]])
                    continue;
                if (!allowed[row_digits[a][2] + row_digits[b][2] + row_digits[c][2]])
                    continue;

                /* diagonals must NOT be in allowed set */
                int diag = row_digits[a][0] + row_digits[b][1] + row_digits[c][2];
                if (allowed[diag]) continue;
                int anti = row_digits[a][2] + row_digits[b][1] + row_digits[c][0];
                if (allowed[anti]) continue;

                int s = (a << 6) | b;     /* state (a,b) */
                int t = (b << 6) | c;     /* next state (b,c) */
                edge_counts[s]++;
            }
        }
    }

    /* Build offset array */
    offset = (int *)malloc((4096 + 1) * sizeof(int));
    offset[0] = 0;
    for (int s = 0; s < 4096; ++s)
        offset[s + 1] = offset[s] + edge_counts[s];
    int total_edges = offset[4096];

    /* Allocate and fill edges_pool */
    edges_pool = (int *)malloc(total_edges * sizeof(int));
    int *ptr = edges_pool;
    for (int a = 0; a < 64; ++a) {
        if (!allowed[row_sum[a]]) continue;
        for (int b = 0; b < 64; ++b) {
            if (!allowed[row_sum[b]]) continue;
            for (int c = 0; c < 64; ++c) {
                if (!allowed[row_sum[c]]) continue;

                if (!allowed[row_digits[a][0] + row_digits[b][0] + row_digits[c][0]]) continue;
                if (!allowed[row_digits[a][1] + row_digits[b][1] + row_digits[c][1]]) continue;
                if (!allowed[row_digits[a][2] + row_digits[b][2] + row_digits[c][2]]) continue;

                int diag = row_digits[a][0] + row_digits[b][1] + row_digits[c][2];
                if (allowed[diag]) continue;
                int anti = row_digits[a][2] + row_digits[b][1] + row_digits[c][0];
                if (allowed[anti]) continue;

                int s = (a << 6) | b;
                int t = (b << 6) | c;
                *ptr++ = t;
            }
        }
    }

    /* Allocate DP arrays */
    cur = (seq_t *)calloc(4096, sizeof(seq_t));
    next_arr = (seq_t *)calloc(4096, sizeof(seq_t));

    /* Initialise cur for arrays of height 3 (i.e. n=1) .
       For each valid triple (a,b,c), increment state (b,c) */
    for (int a = 0; a < 64; ++a) {
        if (!allowed[row_sum[a]]) continue;
        for (int b = 0; b < 64; ++b) {
            if (!allowed[row_sum[b]]) continue;
            for (int c = 0; c < 64; ++c) {
                if (!allowed[row_sum[c]]) continue;

                if (!allowed[row_digits[a][0] + row_digits[b][0] + row_digits[c][0]]) continue;
                if (!allowed[row_digits[a][1] + row_digits[b][1] + row_digits[c][1]]) continue;
                if (!allowed[row_digits[a][2] + row_digits[b][2] + row_digits[c][2]]) continue;

                int diag = row_digits[a][0] + row_digits[b][1] + row_digits[c][2];
                if (allowed[diag]) continue;
                int anti = row_digits[a][2] + row_digits[b][1] + row_digits[c][0];
                if (allowed[anti]) continue;

                int state = (b << 6) | c;
                cur[state] += 1;
            }
        }
    }

    /* Compute a(1) = total of cur */
    seq_t first_term = 0;
    for (int s = 0; s < 4096; ++s)
        first_term += cur[s];

    /* Initialise cache */
    cache_cap = 16;
    cache = (seq_t *)malloc(cache_cap * sizeof(seq_t));
    cache[0] = first_term;
    cache_len = 1;
    current_n = 1;
}

/* ----------  Advance DP by one row  --------------------------------- */
static seq_t advance_and_get_new_value(void)
{
    memset(next_arr, 0, 4096 * sizeof(seq_t));

    for (int s = 0; s < 4096; ++s) {
        seq_t val = cur[s];
        if (!val) continue;
        int start = offset[s];
        int end   = offset[s + 1];
        for (int i = start; i < end; ++i) {
            int t = edges_pool[i];
            next_arr[t] += val;
        }
    }

    /* Sum over all states to obtain a(n+1) */
    seq_t sum_new = 0;
    for (int i = 0; i < 4096; ++i)
        sum_new += next_arr[i];

    /* Prepare for next iteration */
    seq_t *tmp = cur;
    cur = next_arr;
    next_arr = tmp;

    return sum_new;
}

/* ----------  Public function  --------------------------------------- */
seq_t a(idx_t n)
{
    if (n == 0) return 0;   /* offset starts at 1 */

    if (!initialized) {
        init_tables();
        initialized = true;
    }

    while (current_n < n) {
        seq_t val = advance_and_get_new_value();
        if (cache_len == cache_cap) {
            cache_cap *= 2;
            cache = (seq_t *)realloc(cache, cache_cap * sizeof(seq_t));
        }
        cache[cache_len++] = val;
        current_n++;
    }

    return cache[n - 1];
}
