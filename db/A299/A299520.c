/*
 * A299520  Number of n X 7 0..1 arrays with every element equal to 0, 3, 4,
 *          5, 6 or 8 king-move adjacent elements, with upper left element
 *          zero.
 * https://oeis.org/A299520
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   computed by dynamic programming over rows for an n×7 0–1 array
 *            where each cell must have 0,3,4,5,6 or 8 equal‑value king‑move
 *            neighbours and the upper‑left cell is 0
 * Range:     u8 n <= 3, u16 n <= 5, u32 n <= 10, u64 n <= 18, u128 n <= 35,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>
#include <stdlib.h>
#include <string.h>

#define ROWLEN   7                     /* number of columns */
#define PATTERN_COUNT ((size_t)1 << ROWLEN)   /* 128 */
#define PATTERNS (PATTERN_COUNT + 1)          /* 129, includes sentinel */
#define SENTINEL (PATTERNS - 1)              /* index 128, represents a missing row */
#define STATECOUNT (PATTERNS * PATTERNS)      /* 129 * 129 = 16641 */

static bool init_done = false;

static uint32_t *next_offset = NULL;   /* size STATECOUNT */
static uint8_t  *next_len    = NULL;   /* size STATECOUNT */
static uint16_t *next_index  = NULL;   /* variable length, total states * transitions */

static bool (*valid_edge)[PATTERNS] = NULL;   /* [prev][curr] = triple_valid(prev,curr,SENTINEL) */

static seq_t *dp_cur = NULL;    /* size STATECOUNT */
static seq_t *dp_next = NULL;   /* size STATECOUNT */

/* cache of a(n), 1‑indexed */
static seq_t *a_cache = NULL;
static idx_t  cache_size = 0;   /* current allocated size (a_cache[0] unused) */
static idx_t  computed_n = 0;   /* highest n whose a(n) is in cache */

/* ------------------------------------------------------------------ */
/* Helper: returns true if child row exists (i.e., not sentinel)      */
static inline bool row_exists(int pat) { return pat < PATTERNS - 1; }

/* ------------------------------------------------------------------ */
/* Count same‑value neighbors for a cell at row 'rowf', column j        */
/* rowf: integer pattern (0..127); if sentinel (128) treat as missing   */
static inline bool valid_cell(int p, int q, int r, int j, int cur)
{
    int cnt = 0;
    bool up_ex   = row_exists(p);
    bool down_ex = row_exists(r);

    /* upper row */
    if (up_ex) {
        if (j > 0 && ((p >> (j - 1)) & 1) == cur) cnt++;
        if (((p >> j) & 1) == cur)                cnt++;
        if (j < ROWLEN - 1 && ((p >> (j + 1)) & 1) == cur) cnt++;
    }
    /* same row */
    if (j > 0 && ((q >> (j - 1)) & 1) == cur) cnt++;
    if (j < ROWLEN - 1 && ((q >> (j + 1)) & 1) == cur) cnt++;
    /* lower row */
    if (down_ex) {
        if (j > 0 && ((r >> (j - 1)) & 1) == cur) cnt++;
        if (((r >> j) & 1) == cur)                cnt++;
        if (j < ROWLEN - 1 && ((r >> (j + 1)) & 1) == cur) cnt++;
    }
    /* allowed counts: 0,3,4,5,6,8  -> forbid 1,2,7 */
    return !(cnt == 1 || cnt == 2 || cnt == 7);
}

/* ------------------------------------------------------------------ */
/* Returns true if the entire middle row (q) satisfies constraints
   for a given triple of rows (p,q,r).                                   */
static inline bool triple_valid(int p, int q, int r)
{
    for (int j = 0; j < ROWLEN; ++j) {
        int cur = (q >> j) & 1;
        if (!valid_cell(p, q, r, j, cur))
            return false;
    }
    return true;
}

/* ------------------------------------------------------------------ */
/* Convert 2‑tuple (prev,curr) into a contiguous state index           */
static inline size_t state_index(int prev, int curr)
{
    return (size_t)prev * PATTERNS + (size_t)curr;
}

/* ------------------------------------------------------------------ */
static void init_globals(void)
{
    if (init_done) return;
    init_done = true;

    /* allocate transition data */
    next_offset = (uint32_t *)malloc(STATECOUNT * sizeof(uint32_t));
    next_len    = (uint8_t  *)malloc(STATECOUNT * sizeof(uint8_t));
    valid_edge  = (bool (*)[PATTERNS])malloc(PATTERNS * PATTERNS * sizeof(bool));

    /* first pass: count transitions per state and compute valid_edge */
    uint32_t total_trans = 0;
    for (int prev = 0; prev < PATTERNS; ++prev) {
        for (int curr = 0; curr < PATTERNS; ++curr) {
            size_t st = state_index(prev, curr);
            uint8_t len = 0;
            for (int r = 0; r < PATTERNS; ++r) {
                if (r == SENTINEL) continue;          /* no real bottom sentinel here */
                if (triple_valid(prev, curr, r))
                    len++;
            }
            next_len[st] = len;
            total_trans += len;
            valid_edge[prev][curr] = triple_valid(prev, curr, SENTINEL);
        }
    }

    /* allocate flat array of next states */
    next_index = (uint16_t *)malloc(total_trans * sizeof(uint16_t));

    /* second pass: fill next_index and fill offsets */
    uint32_t pos = 0;
    for (int prev = 0; prev < PATTERNS; ++prev) {
        for (int curr = 0; curr < PATTERNS; ++curr) {
            size_t st = state_index(prev, curr);
            next_offset[st] = pos;
            for (int r = 0; r < PATTERNS; ++r) {
                if (r == SENTINEL) continue;
                if (triple_valid(prev, curr, r)) {
                    next_index[pos++] = (uint16_t)state_index(curr, r);
                }
            }
        }
    }

    /* allocate DP arrays */
    dp_cur  = (seq_t *)malloc(STATECOUNT * sizeof(seq_t));
    dp_next = (seq_t *)malloc(STATECOUNT * sizeof(seq_t));

    /* zero initialize DP arrays */
    memset(dp_cur, 0, STATECOUNT * sizeof(seq_t));
    memset(dp_next, 0, STATECOUNT * sizeof(seq_t));
}

/* ------------------------------------------------------------------ */
/* Ensure cache large enough for indices up to n (1‑indexed).           */
static void ensure_cache(idx_t n)
{
    if (n + 1 <= cache_size) return;   /* already large enough */
    size_t new_size = n + 1;
    seq_t *new_cache = (seq_t *)malloc(new_size * sizeof(seq_t));
    if (cache_size > 0)
        memcpy(new_cache, a_cache, cache_size * sizeof(seq_t));
    for (size_t i = cache_size; i < new_size; ++i)
        new_cache[i] = 0;
    free(a_cache);
    a_cache = new_cache;
    cache_size = new_size;
}

/* ------------------------------------------------------------------ */
static void compute_up_to(idx_t n)
{
    if (n == 0) return;
    init_globals();
    ensure_cache(n);

    if (computed_n >= n) return;   /* nothing more to do */

    /* ---- initialisation for n = 1 ---- */
    if (computed_n == 0) {
        /* only the first row exists. Its upper neighbours are missing,
           so we initialise states (SENTINEL, first_row). */
        for (int first = 0; first < PATTERN_COUNT; ++first) {
            if ((first & 1) != 0) continue;   /* upper-left must be 0 */
            dp_cur[state_index(SENTINEL, first)] = 1;
        }
        /* compute a(1) by validating the sole row with SENTINEL below */
        seq_t sum = 0;
        for (int first = 0; first < PATTERN_COUNT; ++first) {
            size_t st = state_index(SENTINEL, first);
            if (!valid_edge[SENTINEL][first]) continue;
            sum += dp_cur[st];
        }
        a_cache[1] = sum;
        computed_n = 1;
    }

    /* ---- proceed for rows 2 .. n ---- */
    for (idx_t cur = computed_n + 1; cur <= n; ++cur) {
        /* clear dp_next */
        memset(dp_next, 0, STATECOUNT * sizeof(seq_t));

        /* transitions: for every current state, distribute counts to next states */
        for (size_t st = 0; st < STATECOUNT; ++st) {
            seq_t val = dp_cur[st];
            if (!val) continue;
            uint32_t start = next_offset[st];
            uint8_t len = next_len[st];
            /* propagate to all possible next rows */
            for (uint8_t k = 0; k < len; ++k) {
                seq_t *cell = &dp_next[next_index[start + k]];
                *cell += val;
            }
        }

        /* swap dp arrays */
        seq_t *tmp = dp_cur;
        dp_cur = dp_next;
        dp_next = tmp;

        /* compute a(cur) by validating the last row with SENTINEL below */
        seq_t sum = 0;
        for (size_t st = 0; st < STATECOUNT; ++st) {
            seq_t val = dp_cur[st];
            if (!val) continue;
            size_t prev = st / PATTERNS;
            size_t curr = st % PATTERNS;
            if (valid_edge[prev][curr]) {
                sum += val;
            }
        }
        a_cache[cur] = sum;
    }
    computed_n = n;
}

/* ------------------------------------------------------------------ */
seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0;   /* undefined offset, return 0 */
    compute_up_to(n);
    return a_cache[n];
}
