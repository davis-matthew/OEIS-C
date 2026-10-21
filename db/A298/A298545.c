/*
 * A298545  Number of nX6 0..1 arrays with every element equal to 1, 2, 3, 4
 *          or 7 king-move adjacent elements, with upper left element zero.
 * https://oeis.org/A298545
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = number of n×6 0..1 arrays with each element having 1, 2,
 *            3, 4 or 7 king‑move neighbors of the same value, upper‑left
 *            element zero
 * Range:     u8 n <= 1, u16 n <= 3, u32 n <= 7, u64 n <= 14, u128 n <= 28,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>

/* helper to check validity of a row given the rows above and below.
   `up` and `down` are either values in [0,63] (actual rows) or the
   sentinel value 64 that represents "no row". */
static bool row_valid(int up, uint8_t mid, int down)
{
    for (int j = 0; j < 6; ++j) {
        int mid_bit = (mid >> j) & 1;
        int cnt = 0;

        /* neighbours in the row above */
        if (up != 64) {
            if (j > 0 && ((up >> (j-1)) & 1) == mid_bit) ++cnt;
            if (j < 5 && ((up >> (j+1)) & 1) == mid_bit) ++cnt;
            if (((up >> j) & 1) == mid_bit) ++cnt;
        }

        /* neighbours in the same row */
        if (j > 0 && ((mid >> (j-1)) & 1) == mid_bit) ++cnt;
        if (j < 5 && ((mid >> (j+1)) & 1) == mid_bit) ++cnt;

        /* neighbours in the row below */
        if (down != 64) {
            if (j > 0 && ((down >> (j-1)) & 1) == mid_bit) ++cnt;
            if (j < 5 && ((down >> (j+1)) & 1) == mid_bit) ++cnt;
            if (((down >> j) & 1) == mid_bit) ++cnt;
        }

        if (!(cnt == 1 || cnt == 2 || cnt == 3 || cnt == 4 || cnt == 7))
            return false;
    }
    return true;
}

static const int STATES = 65 * 64;      /* up (0..64) × mid (0..63) */
static int *edge_start = NULL;
static int *edge_count = NULL;
static int *edge_dest  = NULL;
static bool *final_valid = NULL;
static seq_t *dp_curr = NULL;
static seq_t *dp_next = NULL;

static seq_t *cache = NULL;          /* array of a(n) for 1..max_n */
static size_t cache_size = 0;        /* current allocated size (in entries) */
static idx_t max_n = 0;              /* highest n computed so far */

/* initialise data structures only once */
static void init(void)
{
    /* allocate transition information */
    edge_start = (int *)malloc(STATES * sizeof(int));
    edge_count = (int *)malloc(STATES * sizeof(int));
    final_valid = (bool *)malloc(STATES * sizeof(bool));

    /* first pass: count outgoing edges and final validity */
    size_t total_edges = 0;
    for (int up = 0; up <= 64; ++up) {
        for (uint8_t mid = 0; mid < 64; ++mid) {
            int idx = up * 64 + mid;
            int cnt = 0;
            for (int down = 0; down < 64; ++down) {
                if (row_valid(up, mid, down)) {
                    ++cnt;
                    ++total_edges;
                }
            }
            edge_count[idx] = cnt;
            /* final check with down sentinel */
            final_valid[idx] = row_valid(up, mid, 64);
        }
    }

    edge_dest = (int *)malloc(total_edges * sizeof(int));

    /* compute start indices */
    int pos = 0;
    for (int i = 0; i < STATES; ++i) {
        edge_start[i] = pos;
        pos += edge_count[i];
    }

    /* second pass: fill destination list */
    pos = 0;
    for (int up = 0; up <= 64; ++up) {
        for (uint8_t mid = 0; mid < 64; ++mid) {
            int idx = up * 64 + mid;
            for (int down = 0; down < 64; ++down) {
                if (row_valid(up, mid, down)) {
                    edge_dest[pos++] = (mid * 64 + down);
                }
            }
        }
    }

    dp_curr = (seq_t *)calloc(STATES, sizeof(seq_t));
    dp_next = (seq_t *)calloc(STATES, sizeof(seq_t));

    /* initialise DP for n = 1   (top‑left bit has to be 0) */
    for (uint8_t mid = 0; mid < 64; ++mid) {
        if ((mid & 1) == 0) {
            dp_curr[64 * 64 + mid] = 1;   /* up = sentinel (64) */
        }
    }

    /* compute a(1) and store it */
    seq_t first = 0;
    for (int s = 0; s < STATES; ++s) {
        if (final_valid[s]) {
            first += dp_curr[s];
        }
    }
    /* allocate cache for a(1) */
    cache_size = 2;          /* indices 0 and 1 */
    cache = (seq_t *)calloc(cache_size, sizeof(seq_t));
    cache[1] = first;
    max_n = 1;
}

seq_t a(idx_t n)
{
    if (n == 0) return 0;   /* offset 1 – behaviour below offset is irrelevant */

    if (max_n == 0) init();   /* first call */

    if (n <= max_n) {
        return cache[n];
    }

    /* extend DP to desired n, storing intermediate a(k) */
    for (idx_t cur = max_n + 1; cur <= n; ++cur) {
        /* transition: dp_curr -> dp_next */
        memset(dp_next, 0, STATES * sizeof(seq_t));
        for (int s = 0; s < STATES; ++s) {
            seq_t v = dp_curr[s];
            if (!v) continue;
            int start = edge_start[s];
            int cnt   = edge_count[s];
            for (int k = 0; k < cnt; ++k) {
                int dest = edge_dest[start + k];
                dp_next[dest] += v;
            }
        }
        /* swap */
        seq_t *tmp = dp_curr;
        dp_curr = dp_next;
        dp_next = tmp;

        /* compute a(cur) from final validity */
        seq_t res = 0;
        for (int s = 0; s < STATES; ++s) {
            if (final_valid[s]) {
                res += dp_curr[s];
            }
        }

        /* store result */
        if (cur >= cache_size) {
            size_t new_size = (cache_size == 0) ? 2 : cache_size * 2;
            while (new_size <= cur) new_size *= 2;
            cache = (seq_t *)realloc(cache, new_size * sizeof(seq_t));
            memset(cache + cache_size, 0, (new_size - cache_size) * sizeof(seq_t));
            cache_size = new_size;
        }
        cache[cur] = res;
    }

    max_n = n;
    return cache[n];
}
