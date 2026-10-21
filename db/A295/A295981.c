/*
 * A295981  Number of nX4 0..1 arrays with each 1 adjacent to 2 or 3 king-move
 *          neighboring 1s.
 * https://oeis.org/A295981
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 3, u16 n <= 6, u32 n <= 12, u64 n <= 25, u128 n <= 50,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  A295981 – Number of n×4 0..1 arrays with each 1 adjacent to 2 or 3
 *  king‑move neighboring 1s.  The sequence is computed by dynamic
 *  programming over the rows.  A state consists of:
 *      mask   – which columns have a 1 in the current row (4 bits)
 *      pc[4]  – for each column the number of neighbour 1s that
 *               have already been counted for the 1 in the current row
 *  The transition table between states is built once; from step 1 to
 *  n we propagate the counts.  The algorithm runs in O(n) with a
 *  constant factor of about 64 k updates per row.  It works with any
 *  integer type supplied for seq_t and idx_t.  Memory allocation
 *  uses new/​delete when compiling as C++ and malloc/calloc when
 *  compiling as C++11/C11. */
 
#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>

#define NUM_STATES 4096          /* 16 masks × 16 possible partial counts */

/* Transition tables (filled once) */
static uint16_t trans[NUM_STATES][16];
static uint8_t  trans_len[NUM_STATES];

/* Build transitions for all states */
static void build_transitions(void)
{
    for (int id = 0; id < NUM_STATES; ++id) {
        int mask = id & 0xF;                     /* current row mask */
        int pc   = id >> 4;                      /* packed per‑col counts */
        int p[4] = { pc & 3, (pc >> 2) & 3,
                     (pc >> 4) & 3, (pc >> 6) & 3 };

        uint8_t len = 0;
        for (int nmask = 0; nmask < 16; ++nmask) {
            bool ok = true;

            /* Check that every 1 in the previous row now has 2 or 3 neighbours. */
            for (int j = 0; j < 4 && ok; ++j) {
                if (mask & (1 << j)) {
                    int contrib_new = (nmask >> j) & 1;
                    if (j > 0)   contrib_new += (nmask >> (j-1)) & 1;
                    if (j < 3)   contrib_new += (nmask >> (j+1)) & 1;
                    int total = p[j] + contrib_new;
                    if (total != 2 && total != 3) ok = false;
                }
            }
            if (!ok) continue;

            /* Compute neighbour counts that will be pending for the new row. */
            int new_p[4];
            for (int j = 0; j < 4; ++j) {
                int above = (mask >> j) & 1;
                if (j > 0)   above += (mask >> (j-1)) & 1;
                if (j < 3)   above += (mask >> (j+1)) & 1;

                int horiz = 0;
                if (j > 0)   horiz += (nmask >> (j-1)) & 1;
                if (j < 3)   horiz += (nmask >> (j+1)) & 1;

                int val = above + horiz;
                if (nmask & (1 << j)) {
                    if (val > 3) { ok = false; break; }
                } else {
                    val = 0;                 /* zeros contribute nothing */
                }
                new_p[j] = val;
            }
            if (!ok) continue;

            int new_pc = (new_p[0] |
                          (new_p[1] << 2) |
                          (new_p[2] << 4) |
                          (new_p[3] << 6));
            uint16_t new_id = nmask | (new_pc << 4);
            trans[id][len++] = new_id;
        }
        trans_len[id] = len;
    }
}

/* State arrays (one per row) */
static seq_t *state_cur  = NULL;
static seq_t *state_next = NULL;
static bool   states_allocated = false;
static void init_states(void)
{
    if (!states_allocated) {
#if defined(__cplusplus)
        state_cur  = new seq_t[NUM_STATES]();
        state_next = new seq_t[NUM_STATES]();
#else
        state_cur  = (seq_t *)calloc(NUM_STATES, sizeof(seq_t));
        state_next = (seq_t *)calloc(NUM_STATES, sizeof(seq_t));
#endif
        states_allocated = true;
    }
}

/* Compute the total number of valid arrays for the current state. */
static seq_t compute_total(void)
{
    seq_t total = 0;
    for (int id = 0; id < NUM_STATES; ++id) {
        seq_t cnt = state_cur[id];
        if (cnt == 0) continue;
        int mask = id & 0xF;
        int pc   = id >> 4;
        int p[4] = { pc & 3, (pc >> 2) & 3,
                     (pc >> 4) & 3, (pc >> 6) & 3 };

        if ((mask & 1)   && (p[0] != 2 && p[0] != 3)) continue;
        if ((mask & 2)   && (p[1] != 2 && p[1] != 3)) continue;
        if ((mask & 4)   && (p[2] != 2 && p[2] != 3)) continue;
        if ((mask & 8)   && (p[3] != 2 && p[3] != 3)) continue;
        total += cnt;
    }
    return total;
}

/* Cache for already computed terms */
static seq_t *cache     = NULL;
static size_t cache_size = 0;
static idx_t  max_n = 0;

/* Grow the cache to hold at least n+1 elements. */
static void ensure_cache(idx_t n)
{
    if (cache_size > (size_t)n) return;
    size_t new_size = n + 1;
    if (new_size < 16) new_size = 16;

#if defined(__cplusplus)
    seq_t *tmp = new seq_t[new_size];
    for (size_t i = 0; i < cache_size; ++i) tmp[i] = cache[i];
    for (size_t i = cache_size; i < new_size; ++i) tmp[i] = (seq_t)0;
    delete[] cache;
    cache = tmp;
    cache_size = new_size;
#else
    seq_t *tmp = (seq_t *)realloc(cache, new_size * sizeof(seq_t));
    if (!tmp) { cache = NULL; cache_size = 0; return; }
    for (size_t i = cache_size; i < new_size; ++i) tmp[i] = (seq_t)0;
    cache = tmp;
    cache_size = new_size;
#endif
}

/* --- Public sequence function ------------------------------------------- */
seq_t a(idx_t n)
{
    if (n < 1) return (seq_t)0;

    /* Initialise once */
    static bool ready = false;
    if (!ready) {
        build_transitions();
        init_states();
        ready = true;
    }

    ensure_cache(n);
    if (!cache) return (seq_t)0;          /* allocation failure */

    if (n <= max_n) return cache[n];

    /* Compute from (max_n+1) up to n */
    for (idx_t step = max_n + 1; step <= n; ++step) {
        if (step == 1) {
            /* Initialise first row: only the current row counts matter. */
            for (int mask = 0; mask < 16; ++mask) {
                int p[4];
                for (int j = 0; j < 4; ++j) {
                    int left  = (j > 0)   ? ((mask >> (j-1)) & 1) : 0;
                    int right = (j < 3)   ? ((mask >> (j+1)) & 1) : 0;
                    p[j] = left + right;                 /* 0..2 */
                }
                int pc = (p[0] | (p[1] << 2) |
                          (p[2] << 4) | (p[3] << 6));
                int id = mask | (pc << 4);
                state_cur[id] = (seq_t)1;
            }
        } else {
            /* Transition to next row */
            for (int i = 0; i < NUM_STATES; ++i) state_next[i] = (seq_t)0;
            for (int id = 0; id < NUM_STATES; ++id) {
                seq_t c = state_cur[id];
                if (c == 0) continue;
                for (uint8_t k = 0; k < trans_len[id]; ++k) {
                    uint16_t nid = trans[id][k];
                    state_next[nid] += c;
                }
            }
            /* Swap current and next */
            seq_t *tmp = state_cur;
            state_cur = state_next;
            state_next = tmp;
        }
        /* Store the total for this number of rows */
        cache[step] = compute_total();
    }

    max_n = n;
    return cache[n];
}
