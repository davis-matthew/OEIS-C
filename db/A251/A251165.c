/*
 * A251165  Number of (n+1)X(6+1) 0..3 arrays with no 2X2 subblock having the
 *          maximum of its diagonal elements greater than the absolute
 *          difference of its antidiagonal elements.
 * https://oeis.org/A251165
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 none, u16 none, u32 n <= 4, u64 n <= 12, u128 n <= 29,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  OEIS A251165
    Number of (n+1)×7 0..3 arrays with no 2×2 subblock having
    the maximum of its diagonal elements greater than the
    absolute difference of its antidiagonal elements.
    This implementation uses a transfer‑matrix approach:
    each row is a 7‑digit base‑4 number (4⁷ = 16384 states).
    Two states r → s are compatible iff for every j = 0..5
        max(r[j] , s[j+1]) ≤ |r[j+1] – s[j]|.
    The number of arrays of height n+1 equals the number of
    directed walks of length n in this graph, starting from any
    row.  The values are computed incrementally and cached
    to allow calls in any order. */

#include <stdlib.h>
#include <string.h>
#include <stdint.h>

#define R (16384)          /* 4⁷ */

static uint8_t row_digits[R][7];   /* digits of each state   */
static size_t *adj_start;          /* adjacency prefix sums  */
static uint16_t *adj_edges;        /* concatenated neighbour lists */
static uint8_t  next_digits[7];    /* temporary buffer for recursion */

/* state vectors (current and next step)                     */
static seq_t *state_cur;           
static seq_t *state_nxt;           
/* cache for a(n) values (index 0 unused)                    */
static seq_t *cache;               
static size_t cache_len;           
/* number of steps already performed (steps_done == cache_len-1)*/
static size_t steps_done;

/* ------------------------------------------------------------------
   Recursively count the number of compatible next states for a given
   previous state.  'cnt' is increased for each leaf.
   ------------------------------------------------------------------ */
static void count_next_rec(const uint8_t *prev, int pos, size_t *cnt)
{
    if (pos == 7) {
        (*cnt)++;
        return;
    }
    if (pos == 0) {
        for (int d = 0; d < 4; ++d) {
            next_digits[0] = (uint8_t)d;
            count_next_rec(prev, 1, cnt);
        }
    } else {
        uint8_t a  = prev[pos-1];
        uint8_t b  = prev[pos];
        uint8_t c  = next_digits[pos-1];
        int diff   = (b > c) ? (int)(b - c) : (int)(c - b);
        for (int d = 0; d < 4; ++d) {
            uint8_t max_ad = (a > d) ? a : d;
            if (max_ad <= diff) {
                next_digits[pos] = (uint8_t)d;
                count_next_rec(prev, pos+1, cnt);
            }
        }
    }
}

/* ------------------------------------------------------------------
   Recursively generate all compatible next state ids and write them
   into 'edges' starting at the offset pointed to by 'idxp'.
   ------------------------------------------------------------------ */
static void gen_next_store(const uint8_t *prev, int pos,
                           uint16_t id_accum, uint16_t *edges,
                           size_t *idxp)
{
    if (pos == 7) {
        edges[(*idxp)++] = id_accum;
        return;
    }
    if (pos == 0) {
        for (int d = 0; d < 4; ++d) {
            next_digits[0] = (uint8_t)d;
            gen_next_store(prev, 1, (uint16_t)((id_accum << 2) | d),
                           edges, idxp);
        }
    } else {
        uint8_t a  = prev[pos-1];
        uint8_t b  = prev[pos];
        uint8_t c  = next_digits[pos-1];
        int diff   = (b > c) ? (int)(b - c) : (int)(c - b);
        for (int d = 0; d < 4; ++d) {
            uint8_t max_ad = (a > d) ? a : d;
            if (max_ad <= diff) {
                next_digits[pos] = (uint8_t)d;
                gen_next_store(prev, pos+1,
                               (uint16_t)((id_accum << 2) | d),
                               edges, idxp);
            }
        }
    }
}

/* ------------------------------------------------------------------
   Build the adjacency lists for the transfer matrix.
   ------------------------------------------------------------------ */
static void build_adjacency(void)
{
    size_t i, j;
    size_t *degree = (size_t *)malloc(R * sizeof(size_t));

    /* 1. Precompute the digits of every state */
    for (i = 0; i < R; ++i) {
        uint32_t tmp = (uint32_t)i;
        for (int pos = 6; pos >= 0; --pos) {
            row_digits[i][pos] = (uint8_t)(tmp & 3u);
            tmp >>= 2;
        }
    }

    /* 2. Count outgoing edges for each state */
    adj_start = (size_t *)malloc((R + 1) * sizeof(size_t));
    adj_start[0] = 0;
    for (i = 0; i < R; ++i) {
        size_t cnt = 0;
        count_next_rec(row_digits[i], 0, &cnt);
        degree[i] = cnt;
        adj_start[i+1] = adj_start[i] + cnt;
    }
    size_t total_edges = adj_start[R];
    adj_edges = (uint16_t *)malloc(total_edges * sizeof(uint16_t));

    /* 3. Fill adjacency lists */
    for (i = 0; i < R; ++i) {
        size_t idx_start = adj_start[i];
        gen_next_store(row_digits[i], 0, 0, adj_edges, &idx_start);
    }

    free(degree);
}

/* ------------------------------------------------------------------
   Initialise static data structures once.
   ------------------------------------------------------------------ */
static void initialize(void)
{
    static int init_done = 0;
    if (init_done) return;
    init_done = 1;

    build_adjacency();

    state_cur  = (seq_t *)malloc(R * sizeof(seq_t));
    state_nxt  = (seq_t *)malloc(R * sizeof(seq_t));
    cache_len  = 2;                    /* indices 0 and 1 */
    cache      = (seq_t *)malloc(cache_len * sizeof(seq_t));
    cache[0]   = (seq_t)0;             /* a(0) not used but defined */
    steps_done = 0;

    /* initial state: one way to start with each possible first row */
    for (size_t i = 0; i < R; ++i)
        state_cur[i] = (seq_t)1;
}

/* ------------------------------------------------------------------
   Return a(n).  The function can receive queries in any order.
   ------------------------------------------------------------------ */
seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0;    /* not defined for n=0, return 0 */

    initialize();

    /* enlarge cache if necessary */
    if ((size_t)n + 1 > cache_len) {
        size_t new_len = (size_t)n + 1;
        seq_t *tmp = (seq_t *)realloc(cache, new_len * sizeof(seq_t));
        if (!tmp) abort();   /* allocation failure */
        cache = tmp;
        cache_len = new_len;
    }

    /* compute a(n) if it hasn't been computed yet */
    while (steps_done < (size_t)n) {
        /* clear next vector */
        memset(state_nxt, 0, R * sizeof(seq_t));

        /* one step of the transfer matrix */
        for (size_t prev = 0; prev < R; ++prev) {
            seq_t val = state_cur[prev];
            if (val == 0) continue;
            for (size_t idx = adj_start[prev]; idx < adj_start[prev+1]; ++idx) {
                state_nxt[adj_edges[idx]] += val;
            }
        }

        /* swap state vectors */
        seq_t *tmp = state_cur;
        state_cur = state_nxt;
        state_nxt = tmp;

        /* compute a(steps_done+1) by summing the new vector */
        seq_t total = 0;
        for (size_t k = 0; k < R; ++k)
            total += state_cur[k];
        cache[steps_done + 1] = total;

        ++steps_done;
    }

    return cache[n];
}
