/*
 * A251340  Number of (n+1)X(5+1) 0..3 arrays with every 2X2 subblock summing
 *          to a nonzero multiple of 3.
 * https://oeis.org/A251340
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 none, u16 none, u32 n <= 4, u64 n <= 10, u128 n <= 21,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* OEIS A251340
   a(n) = number of (n+1) × 6 arrays with entries 0..3 such that every 2×2
   subblock sums to a non‑zero multiple of 3. */

/* The implementation uses a dynamic‑programming approach over all 4^6
   possible rows.  For each pair of rows r and s we pre‑compute
   compatibility using the 2×2 sum rule.  The DP vector v_t represents
   the number of ways to reach each row after t transitions (t = n).
   After each transition we sum the vector to obtain a(t).  The
   algorithm is O(total_edges × n) with a very small constant
   (~65 000 edges). */

#include <stddef.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>

#define ROWS 4096   /* 4^6 */

/* Forward declarations of the supplied integer types. */
typedef idx_t idx_t;   /* uint64_t in the build */
typedef seq_t seq_t;   /* unsigned or arbitrary‑precision */

/* Row representation: each row is encoded as 6 2‑bit digits. */
static unsigned char row_vals[ROWS][6];

/* Adjacency list: neighbors[r] contains all rows compatible with r. */
static uint16_t *adj = NULL;          /* flattened neighbor list */
static size_t *adj_start = NULL;      /* start index of each row */
static size_t *adj_len   = NULL;      /* number of neighbors per row */
static size_t total_edges = 0;

/* DP vectors. */
static seq_t *dp_cur = NULL;
static seq_t *dp_next = NULL;

/* Cache of results a(1)…a(n).  cache[0] is unused. */
static seq_t *cache = NULL;
static size_t cache_capacity = 0;
static idx_t max_cached = 0;

/* ------------------------------------------------------------ */
/* Utility: test if two rows are compatible.  For each of the
   five 2×2 blocks the sum of the four entries must be 3,6,9, or 12. */
static int compatible(uint16_t r, uint16_t s)
{
    int j;
    for (j = 0; j < 5; ++j) {
        int p = row_vals[r][j] + row_vals[r][j + 1];
        int q = row_vals[s][j] + row_vals[s][j + 1];
        int sum = p + q;
        if (sum % 3 != 0 || sum == 0)
            return 0;
    }
    return 1;
}

/* ------------------------------------------------------------ */
/* One‑time initialization of all arrays. */
static void init(void)
{
    if (dp_cur != NULL) return;  /* already initialized */

    /* Build row values table. */
    size_t r;
    for (r = 0; r < ROWS; ++r) {
        uint32_t x = (uint32_t)r;   /* r < 4096 fits in 12 bits */
        size_t col;
        for (col = 0; col < 6; ++col) {
            row_vals[r][col] = (unsigned char)(x & 3);
            x >>= 2;
        }
    }

    /* Count edges to know required memory. */
    for (r = 0; r < ROWS; ++r) {
        size_t s;
        for (s = 0; s < ROWS; ++s) {
            if (compatible((uint16_t)r, (uint16_t)s))
                ++total_edges;
        }
    }

    /* Allocate adjacency structures. */
    adj = (uint16_t *)malloc(total_edges * sizeof(uint16_t));
    adj_start = (size_t *)malloc((ROWS + 1) * sizeof(size_t));
    adj_len   = (size_t *)malloc(ROWS * sizeof(size_t));

    size_t pos = 0;
    for (r = 0; r < ROWS; ++r) {
        adj_start[r] = pos;
        size_t cnt = 0;
        for (size_t s = 0; s < ROWS; ++s) {
            if (compatible((uint16_t)r, (uint16_t)s)) {
                adj[pos++] = (uint16_t)s;
                ++cnt;
            }
        }
        adj_len[r] = cnt;
    }
    adj_start[ROWS] = pos;   /* sentinel */

    /* Allocate DP vectors. */
    dp_cur  = (seq_t *)calloc(ROWS, sizeof(seq_t));
    dp_next = (seq_t *)calloc(ROWS, sizeof(seq_t));
    for (r = 0; r < ROWS; ++r)
        dp_cur[r] = (seq_t)1;   /* first row can be anything */

    /* Initialise cache. */
    cache_capacity = 1;
    cache = (seq_t *)malloc(cache_capacity * sizeof(seq_t));
    /* cache[0] unused. */
}

/* ------------------------------------------------------------ */
/* Ensure cache array can hold values up to index n. */
static void ensure_cache(idx_t n)
{
    if ((size_t)n < cache_capacity)
        return;
    size_t new_cap = cache_capacity ? cache_capacity * 2 : 1;
    while ((size_t)n >= new_cap)
        new_cap <<= 1;
    seq_t *new_cache = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
    cache = new_cache;
    cache_capacity = new_cap;
}

/* ------------------------------------------------------------ */
/* Main sequence function. */
seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0;   /* out of offset */

    init();
    ensure_cache(n);

    /* Compute up to n if not already done. */
    for (idx_t step = max_cached + 1; step <= n; ++step) {
        /* Zero dp_next vector. */
        memset(dp_next, 0, ROWS * sizeof(seq_t));

        /* For each current row, propagate to its neighbors. */
        seq_t total = (seq_t)0;
        for (size_t r = 0; r < ROWS; ++r) {
            seq_t val = dp_cur[r];
            if (val == (seq_t)0)
                continue;
            size_t pos   = adj_start[r];
            size_t count = adj_len[r];
            while (count--) {
                uint16_t nb = adj[pos++];
                dp_next[nb] += val;
                total += val;
            }
        }

        /* Swap dp vectors for next iteration. */
        seq_t *tmp = dp_cur;
        dp_cur = dp_next;
        dp_next = tmp;

        /* Store result for this step. */
        cache[(size_t)step] = total;
    }

    max_cached = n;
    return cache[(size_t)n];
}
