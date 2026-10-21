/*
 * A250543  Number of (n+1)X(7+1) 0..3 arrays with nondecreasing
 *          x(i,j)-x(i,j-1) in the i direction and nondecreasing
 *          x(i,j)-x(i-1,j) in the j direction.
 * https://oeis.org/A250543
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 none, u16 none, u32 n <= 4, u64 n <= 20, u128 n <= 30,
 *            big any n
 * Helpers:   add_edge
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*
 * Formula: a(n) = number of (n+1)×8 arrays with entries 0..3 such that
 *     horizontally the differences x(i,j)-x(i,j-1) are nondecreasing in i
 *     and vertically the differences x(i,j)-x(i-1,j) are nondecreasing in j.
 *
 * The implementation builds all transitions between consecutive rows,
 * precomputes a(1) … a(16) by dynamic programming, then uses the
 * empirical 16‑term linear recurrence for larger n.
 */

#include <stdint.h>
#include <stddef.h>
#include <stdbool.h>
#include <stdlib.h>
#include <string.h>
#include "../utils/add_edge.h"

/* ------------------------------------------------------------------ */
/*  Data structures for the adjacency lists                           */
/* ------------------------------------------------------------------ */
typedef struct {
    uint16_t *data;
    size_t len;
    size_t cap;
} neigh_t;

/* ------------------------------------------------------------------ */
/*  Global static state                                               */
/* ------------------------------------------------------------------ */
static bool _global_inited = false;

static neigh_t *adjacency = NULL;      /* adjacency[prevRow] -> list of nextRows */
static size_t _state_cnt = 0;          /* number of possible rows (4^8 = 65536) */

static seq_t *a_cache = NULL;          /* cached sequence values, a_cache[1] = a(1) */
static size_t a_cap   = 0;             /* capacity of a_cache */
static size_t a_len   = 0;             /* number of cached terms (a(1)..a(a_len)) */

/* ------------------------------------------------------------------ */
/*  Helper for adding an item to a growing array                      */

/* ------------------------------------------------------------------ */
/*  Recursively generate all admissible vertical difference sequences */
/* ------------------------------------------------------------------ */
static void gen_sequences(int pos, int last_val,
                          int prev_idx,
                          const int8_t *p,
                          const int8_t *lo,
                          const int8_t *hi,
                          int8_t *dcur,
                          neigh_t *adj)
{
    if (pos == 8) {
        /* build successor row code */
        uint16_t code = 0;
        for (int j = 7; j >= 0; --j) {
            int curr = p[j] + dcur[j];        /* 0..3 guaranteed by bounds */
            code |= (uint16_t)(curr << (2 * j));
        }
        add_edge(&adj[prev_idx].data, &adj[prev_idx].len,
                 &adj[prev_idx].cap, code);
        return;
    }
    int start = lo[pos];
    if (pos > 0 && start < last_val) start = last_val;
    int end = hi[pos];
    for (int val = start; val <= end; ++val) {
        dcur[pos] = (int8_t)val;
        gen_sequences(pos + 1, val, prev_idx, p, lo, hi, dcur, adj);
    }
}

/* ------------------------------------------------------------------ */
/*  Build all row codes (4^8 rows) and the adjacency lists           */
/* ------------------------------------------------------------------ */
static void build_adjacency(void)
{
    _state_cnt = ((size_t)1) << 16;        /* 4^8 = 65536 */
    adjacency = (neigh_t *)calloc(_state_cnt, sizeof(neigh_t));

    int8_t p[8];
    int8_t lo[8];
    int8_t hi[8];
    int8_t d[8];

    for (size_t code = 0; code < _state_cnt; ++code) {
        /* decode current row digits */
        for (int j = 0; j < 8; ++j)
            p[j] = (int8_t)((code >> (2 * j)) & 3U);

        /* compute bounds for d[j] = curr[j] - prev[j] */
        for (int j = 0; j < 8; ++j) {
            lo[j] = -p[j];          /* min possible difference */
            hi[j] = 3 - p[j];       /* max possible difference */
        }
        gen_sequences(0, 0, (int)code, p, lo, hi, d, adjacency);
    }
}

/* ------------------------------------------------------------------ */
/*  DP to compute the first 16 terms                                  */
/* ------------------------------------------------------------------ */
static void compute_first_16_terms(seq_t **out_terms)
{
    /* dp arrays */
    seq_t *dp_cur = (seq_t *)calloc(_state_cnt, sizeof(seq_t));
    seq_t *dp_nxt = (seq_t *)calloc(_state_cnt, sizeof(seq_t));

    /* initial row counts are all 1 */
    for (size_t i = 0; i < _state_cnt; ++i) dp_cur[i] = (seq_t)1;

    seq_t *terms = *out_terms;
    /* Terms[1..16] will be filled */
    for (int step = 1; step <= 16; ++step) {
        /* zero next dp */
        memset(dp_nxt, 0, _state_cnt * sizeof(seq_t));
        /* transition */
        for (size_t prev = 0; prev < _state_cnt; ++prev) {
            seq_t val = dp_cur[prev];
            if (val == (seq_t)0) continue;
            neigh_t *nl = &adjacency[prev];
            for (size_t k = 0; k < nl->len; ++k) {
                dp_nxt[nl->data[k]] += val;
            }
        }
        /* sum all entries to obtain a(step) */
        seq_t sum = 0;
        for (size_t i = 0; i < _state_cnt; ++i) sum += dp_nxt[i];
        terms[step] = sum;
        /* swap */
        seq_t *tmp = dp_cur;
        dp_cur = dp_nxt;
        dp_nxt = tmp;
    }

    /* free temporary arrays */
    free(dp_cur);
    free(dp_nxt);
}

/* ------------------------------------------------------------------ */
/*  Ensure capacity of a_cache                                         */
/* ------------------------------------------------------------------ */
static void ensure_a_capacity(size_t needed)
{
    if (needed < a_cap) return;
    size_t newcap = a_cap ? a_cap : 16;
    while (newcap <= needed) newcap <<= 1;
    seq_t *newmem = (seq_t *)realloc(a_cache, (newcap + 1) * sizeof(seq_t));
    /* zero new area if any */
    if (newcap > a_cap)
        memset(newmem + a_cap + 1, 0, (newcap - a_cap) * sizeof(seq_t));
    a_cache = newmem;
    a_cap = newcap;
}

/* ------------------------------------------------------------------ */
/*  Empirical 16‑term linear recurrence (coefficients given)          */
/* ------------------------------------------------------------------ */
static const int32_t coeff[16] = {
    30, -415, 3514, -20386,
    85924, -272198, 661180, -1244717,
    1822478, -2068955, 1802474, -1181760,
    563888, -184752, 37152, -3456
};

static void compute_more_terms(idx_t n)
{
    /* n > a_len */
    ensure_a_capacity((size_t)n);
    while (a_len < (size_t)n) {
        seq_t pos_sum = 0;
        seq_t neg_sum = 0;
        for (int i = 1; i <= 16; ++i) {
            int32_t c = coeff[i - 1];
            seq_t prod = a_cache[a_len + 1 - i] *
                         (seq_t)(c >= 0 ? c : -c);
            if (c > 0)
                pos_sum += prod;
            else
                neg_sum += prod;
        }
        a_cache[++a_len] = pos_sum - neg_sum;
    }
}

/* ------------------------------------------------------------------ */
/*  Public sequence function                                         */
/* ------------------------------------------------------------------ */
seq_t a(idx_t n)
{
    /* Lazily init global state */
    if (!_global_inited) {
        build_adjacency();

        /* Allocate cache for first 16 terms (index 0 unused) */
        a_cap = 16;
        a_cache = (seq_t *)calloc(a_cap + 1, sizeof(seq_t));
        /* compute first 16 terms */
        compute_first_16_terms(&a_cache);
        a_len = 16;
        _global_inited = true;
    }

    if ((size_t)n <= a_len)
        return a_cache[(size_t)n];

    compute_more_terms((idx_t)n);
    return a_cache[(size_t)n];
}
