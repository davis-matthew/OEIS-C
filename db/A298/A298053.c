/*
 * A298053  Number of nX6 0..1 arrays with every element equal to 1, 2, 4 or 6
 *          king-move adjacent elements, with upper left element zero.
 * https://oeis.org/A298053
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = number of n×6 0/1 arrays in which each entry is 1, 2, 4
 *            or 6 plus‑minus the king‑move neighbours, with the upper‑left
 *            entry forced to 0 (a(1)=5, a(2)=49, …). The implementation uses
 *            a transfer‑matrix dynamic program over pairs of consecutive
 *            rows, memoising all intermediate results
 * Range:     u8 n <= 5, u16 n <= 13, u32 n <= 29, u64 n <= 61, u128 n <= 126,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include <stdint.h>
#include <stdlib.h>

#define WIDTH          6U
#define PATTERNS      (1U << WIDTH)          /* 64 */

/* sentinel for “no row” (used for the topmost and bottommost
   virtual neighbours that contain only zeros).  Value PATTERNS
   lies just beyond the set of real patterns. */
#define NONE            PATTERNS              /* 64 */

#define MAX_STATES     ((PATTERNS + 1) * PATTERNS)   /* 65 × 64 = 4160 */

/* State characterised by (above, current) pair of patterns. */
struct State {
    idx_t above;
    idx_t mid;
};

/* Global tables – built lazily on first call. */
static int id_map[PATTERNS + 1][PATTERNS];          /* [above][mid] -> state id */
static struct State states[MAX_STATES];            /* id → state pair */
static int next_ids[MAX_STATES][PATTERNS];         /* adjacency lists */
static unsigned char next_cnt[MAX_STATES];         /* number of neighbours */
static bool final_ok[MAX_STATES];                  /* true if state can finish */
static int state_count = 0;                        /* reachable states so far */
static bool transitions_built = false;

/* DP buffers.  Size equals state_count after the transition graph
   is constructed. */
static seq_t cur[MAX_STATES];
static seq_t nxt[MAX_STATES];

/* Cache of a( n ) values.  cache[i] holds a(i).  0 is never a
   valid value for i ≥ 1, so a(0) is undefined (the harness never
   calls it). */
static seq_t *cache = NULL;
static size_t cache_allocated = 0;
static idx_t cache_limit    = 0;   /* highest n that has been computed */

/* ------------------------------------------------------------------ */
/* Test whether a row with pattern 'mid' satisfies the adjacency rule
   when the row above is 'above' (or NONE) and the row below is
   'below' (or NONE).  A neighbour counts if it has the same bit
   value as the centre cell. */
static bool row_ok(idx_t above, uint64_t mid, idx_t below)
{
    /* allowed counts of equal king‑move neighbours */
    static const bool allowed[9] = {
        false, true, true, false, true, false, true, false, false };

    for (idx_t col = 0; col < WIDTH; ++col) {
        uint64_t mid_bit = (mid >> col) & 1ULL;
        unsigned cnt = 0;

        /* Same row neighbours */
        if (col > 0)
            cnt += ((mid >> (col - 1)) & 1ULL) == mid_bit;
        if (col + 1 < WIDTH)
            cnt += ((mid >> (col + 1)) & 1ULL) == mid_bit;

        /* Row above */
        if (above != NONE) {
            uint64_t arow = (uint64_t)above;
            if (col > 0)
                cnt += ((arow >> (col - 1)) & 1ULL) == mid_bit;
            if (col + 1 < WIDTH)
                cnt += ((arow >> (col + 1)) & 1ULL) == mid_bit;
            cnt += ((arow >> col) & 1ULL) == mid_bit;
        }

        /* Row below */
        if (below != NONE) {
            uint64_t brow = (uint64_t)below;
            if (col > 0)
                cnt += ((brow >> (col - 1)) & 1ULL) == mid_bit;
            if (col + 1 < WIDTH)
                cnt += ((brow >> (col + 1)) & 1ULL) == mid_bit;
            cnt += ((brow >> col) & 1ULL) == mid_bit;
        }

        if (!allowed[cnt])
            return false;
    }
    return true;
}

/* ------------------------------------------------------------------ */
/* Build the finite state graph and the adjacency lists exactly once. */
static void build_transitions(void)
{
    if (transitions_built)
        return;

    /* Initialise id_map to –1.  Use a small integer type. */
    for (int i = 0; i <= PATTERNS; ++i)
        for (int j = 0; j < PATTERNS; ++j)
            id_map[i][j] = -1;

    /* BFS queue: use a simple fixed array. */
    int queue[MAX_STATES];
    int head = 0, tail = 0;

    /* Seed with all legal first rows (the upper‑left bit is 0). */
    for (idx_t mid = 0; mid < PATTERNS; ++mid) {
        if ((mid & 1ULL) == 0) {
            int id = state_count++;
            id_map[NONE][mid] = id;
            states[id].above = NONE;
            states[id].mid   = mid;
            queue[tail++] = id;
        }
    }

    /* BFS to discover all reachable states. */
    while (head < tail) {
        int s = queue[head++];
        idx_t above = states[s].above;
        idx_t mid   = states[s].mid;
        next_cnt[s] = 0;

        for (idx_t below = 0; below < PATTERNS; ++below) {
            if (!row_ok(above, mid, below))
                continue;

            int nid = id_map[mid][below];
            if (nid < 0) {            /* new state discovered */
                nid = state_count++;
                id_map[mid][below] = nid;
                states[nid].above = mid;
                states[nid].mid   = below;
                queue[tail++] = nid;
            }
            next_ids[s][next_cnt[s]++] = nid;
        }
    }

    /* Mark which states can be final (no row below). */
    for (int s = 0; s < state_count; ++s)
        final_ok[s] = row_ok(states[s].above, (uint64_t)states[s].mid, NONE);

    transitions_built = true;
}

/* ------------------------------------------------------------------ */
/* Grow cached sequence values up to index n (n ≥ 1). */
static void ensure_cache(idx_t n)
{
    if (n <= cache_limit)
        return;                           /* already known */

    /* Grow cache array if needed. */
    if (!cache || (size_t)(n + 1) > cache_allocated) {
        size_t new_size = (size_t)(n + 1);
        seq_t *tmp = (seq_t *)realloc(cache, new_size * sizeof(seq_t));
        if (!tmp)
            exit(1);
        cache = tmp;
        for (size_t i = cache_allocated; i < new_size; ++i)
            cache[i] = 0;
        cache_allocated = new_size;
    }

    /* Build transition graph on first use. */
    if (!transitions_built) {
        build_transitions();

        /* Initial DP for n = 1 (only the first row). */
        for (int i = 0; i < state_count; ++i)
            cur[i] = 0;
        for (int i = 0; i < state_count; ++i)
            if (states[i].above == NONE)
                cur[i] = 1;

        /* Save a(1). */
        seq_t ans = 0;
        for (int i = 0; i < state_count; ++i)
            if (final_ok[i])
                ans += cur[i];
        cache[1] = ans;
        cache_limit = 1;
    }

    /* Extend DP up to the requested n. */
    for (idx_t rows = cache_limit; rows < n; ++rows) {
        /* Compute nr = cur → nxt for the next row. */
        for (int i = 0; i < state_count; ++i)
            nxt[i] = 0;
        for (int i = 0; i < state_count; ++i) {
            seq_t ways = cur[i];
            if (!ways)
                continue;
            for (int j = 0; j < next_cnt[i]; ++j)
                nxt[next_ids[i][j]] += ways;
        }
        /* Swap buffers. */
        for (int i = 0; i < state_count; ++i)
            cur[i] = nxt[i];

        /* Compute a(rows+1). */
        seq_t ans = 0;
        for (int i = 0; i < state_count; ++i)
            if (final_ok[i])
                ans += cur[i];
        cache[rows + 1] = ans;
    }
    cache_limit = n;
}

/* ------------------------------------------------------------------ */
/* Public entry point required by the OEIS harness. */
seq_t a(idx_t n)
{
    if (n == 0)               /* defined offset is 1; out of range */
        return (seq_t)0;
    ensure_cache(n);
    return cache[n];
}
