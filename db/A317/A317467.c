/*
 * A317467  Squares visited by a (2,2)-leaper on a spirally numbered board and
 *          moving to the lowest available unvisited square at each step,
 *          squares labelled >=1.
 * https://oeis.org/A317467
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 16, u16 n <= 4096, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   key_from_xy, spiral_number
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* A317467 – Squares visited by a (2,2)-leaper on a spirally numbered board.
   The function simulates the leaper: at each step it moves to the
   lowest-numbered unvisited square reachable by a (±2,±2) move.
   a(1) = 1, a(n) for n≥1 is the number printed on that square.
   The implementation keeps a hash set of visited (x,y) pairs and
   a cache of already computed sequence values.  It uses only
   integer arithmetic, no floating point, and relies on the fact that
   the spiral number fits in 64 bits for n≤10 000 .  For larger n the
   algorithm still works but the numbers may overflow.
*/

#include <stdlib.h>
#include <stdbool.h>
#include <stdint.h>
#include "../utils/key_from_xy.h"
#include "../utils/spiral_number.h"

/* ---------- hash set of visited positions ------------ */
#define VIS_TABLE_SIZE 32768   /* power of two, > 2× expected visited squares */

static uint64_t *vis_keys = NULL;
static size_t   vis_mask = 0;

/* initialise the hash table */
static void vis_init(void)
{
    if (vis_keys) return;
    vis_mask = VIS_TABLE_SIZE - 1;
#if defined(__cplusplus)
    vis_keys = (uint64_t*)calloc(VIS_TABLE_SIZE, sizeof(uint64_t));
#else
    vis_keys = calloc(VIS_TABLE_SIZE, sizeof(uint64_t));
#endif
}

/* lookup key in the table */
static inline bool vis_contains(uint64_t key)
{
    size_t idx = key & vis_mask;
    while (vis_keys[idx]) {
        if (vis_keys[idx] == key) return true;
        idx = (idx + 1) & vis_mask;
    }
    return false;
}

/* insert key into the table */
static inline void vis_insert(uint64_t key)
{
    size_t idx = key & vis_mask;
    while (vis_keys[idx]) {
        idx = (idx + 1) & vis_mask;
    }
    vis_keys[idx] = key;
}

/* ------------- sequence cache ---------------------- */
static uint64_t *seq_vals = NULL;
static size_t seq_len  = 0;
static size_t seq_cap  = 0;

static int32_t cur_x = 0;
static int32_t cur_y = 0;

/* enlarge the cache if required */
static void seq_reserve(size_t need)
{
    if (seq_cap >= need) return;
    size_t new_cap = seq_cap ? seq_cap * 2 : 16;
    if (new_cap < need) new_cap = need * 2;
#if defined(__cplusplus)
    seq_vals = (uint64_t*)realloc(seq_vals, new_cap * sizeof(uint64_t));
#else
    seq_vals = realloc(seq_vals, new_cap * sizeof(uint64_t));
#endif
    seq_cap = new_cap;
}

/* ----------------------------------------------------
   Main function required by the build system.
   ---------------------------------------------------- */
seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0;           /* safety for offset < 1 */

    /* lazy initialisation on first use */
    if (seq_len == 0) {
        vis_init();
        seq_reserve(16);
        seq_vals[0] = 1ULL;                /* a(1) = 1 */
        seq_len = 1;
        cur_x = 0;
        cur_y = 0;
        vis_insert(key_from_xy(cur_x, cur_y));
    }

    /* extend cache up to the requested index */
    while ((size_t)n > seq_len) {
        /* generate the four candidate moves */
        int32_t candidates[4][2] = {
            {cur_x + 2, cur_y + 2},
            {cur_x - 2, cur_y + 2},
            {cur_x + 2, cur_y - 2},
            {cur_x - 2, cur_y - 2}
        };

        uint64_t best_val = UINT64_MAX;
        int32_t best_x = 0, best_y = 0;

        for (int i = 0; i < 4; ++i) {
            int32_t nx = candidates[i][0];
            int32_t ny = candidates[i][1];
            uint64_t key = key_from_xy(nx, ny);
            if (vis_contains(key)) continue;
            uint64_t val = spiral_number(nx, ny);
            if (val < best_val) {
                best_val = val;
                best_x = nx;
                best_y = ny;
            }
        }

        cur_x = best_x;
        cur_y = best_y;
        vis_insert(key_from_xy(cur_x, cur_y));

        seq_reserve(seq_len + 1);
        seq_vals[seq_len] = best_val;
        ++seq_len;
    }

    return (seq_t)seq_vals[n - 1];
}
