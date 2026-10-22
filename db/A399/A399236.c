/*
 * A399236  Squares visited by knight moves on a diagonally numbered Q1 board
 *          and always taking the available unvisited square with the shortest
 *          distance to the origin, while in the case of a tie the square with
 *          smaller row index is preferred. Starting square is labeled 1.
 * https://oeis.org/A399236
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, fini, full, new
 * Formula:   not stated
 * Range:     u8 n <= 133, u16 n <= 417, u32 n <= 417, u64 n <= 417,
 *            u128 n <= 417, big any n
 * Helpers:   encode_key
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Algorithm: simulate a knight on the Q1 board, always moving to the
   nearest unvisited square (by Euclidean distance to (0,0));
   tie‑break by smaller row index.  The board squares are numbered
   as 1 + (i+j)*(i+j+1)/2 + i (with i=j=0 at the origin).  The
   function returns the label of the square visited at step n
   (a(1)=1) for all n≥1. */

#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>
#include "../utils/encode_key.h"

/* --- global state ------------------------------------------------------- */
static idx_t cur_i = 0;           /* current row of the knight */
static idx_t cur_j = 0;           /* current column of the knight */

/* visited set (open addressing, 64‑bit keys) */
static uint64_t *visited_keys = NULL;
static idx_t visited_cap   = 0;   /* table capacity, power of two */
static idx_t visited_used  = 0;   /* number of occupied slots */

static const uint64_t EMPTY_KEY = 0xFFFFFFFFFFFFFFFFULL;

/* simple 64‑bit hash (xorshift) */
static inline idx_t hash_key(uint64_t key, idx_t cap)
{
    key ^= key >> 33;
    key *= 0xff51afd7ed558ccdULL;
    key ^= key >> 33;
    key *= 0xc4ceb9fe1a85ec53ULL;
    key ^= key >> 33;
    return (idx_t)key & (cap - 1);
}

/* grow the visited table */
static void visited_expand(void)
{
    idx_t new_cap = visited_cap << 1;
    uint64_t *new_tbl = (uint64_t *)calloc((size_t)new_cap,
                                            sizeof(uint64_t));
    for (idx_t k = 0; k < new_cap; ++k)
        new_tbl[k] = EMPTY_KEY;
    /* rehash existing keys */
    for (idx_t h = 0; h < visited_cap; ++h) {
        uint64_t k = visited_keys[h];
        if (k != EMPTY_KEY) {
            idx_t pos = hash_key(k, new_cap);
            while (new_tbl[pos] != EMPTY_KEY)
                pos = (pos + 1) & (new_cap - 1);
            new_tbl[pos] = k;
        }
    }
    free(visited_keys);
    visited_keys = new_tbl;
    visited_cap  = new_cap;
}

/* initialise visited table with a small table; expected approximated size */
static void visited_init(idx_t expected)
{
    visited_cap = 16;
    while (visited_cap < expected * 2)
        visited_cap <<= 1;
    visited_keys = (uint64_t *)calloc((size_t)visited_cap,
                                      sizeof(uint64_t));
    for (idx_t k = 0; k < visited_cap; ++k)
        visited_keys[k] = EMPTY_KEY;
}

/* membership test */
static bool visited_contains(uint64_t key)
{
    if (!visited_keys)
        return false;
    idx_t pos = hash_key(key, visited_cap);
    while (visited_keys[pos] != EMPTY_KEY) {
        if (visited_keys[pos] == key)
            return true;
        pos = (pos + 1) & (visited_cap - 1);
    }
    return false;
}

/* insert key */
static void visited_insert(uint64_t key)
{
    if (!visited_keys)
        visited_init(16);
    if (visited_used > visited_cap * 7 / 10)
        visited_expand();
    idx_t pos = hash_key(key, visited_cap);
    while (visited_keys[pos] != EMPTY_KEY) {
        if (visited_keys[pos] == key)
            return;
        pos = (pos + 1) & (visited_cap - 1);
    }
    visited_keys[pos] = key;
    ++visited_used;
}

/* --- sequence storage -------------------------------------------------- */
static seq_t *seq  = NULL;      /* array of visited labels */
static idx_t seq_cap = 0;       /* capacity of seq array */
static idx_t seq_len = 0;       /* number of computed terms */

/* push a value to the sequence, expanding if necessary */
static void seq_push(seq_t val)
{
    if (seq_len == seq_cap) {
        idx_t new_cap = seq_cap ? seq_cap * 2 : 16;
        seq_t *tmp = (seq_t *)realloc(seq, (size_t)new_cap * sizeof(seq_t));
        seq = tmp;
        seq_cap = new_cap;
    }
    seq[seq_len++] = val;
}

/* knight move offsets (i,j) */
static const int di[8] = {  1,  1,  2,  2, -1, -1, -2, -2 };
static const int dj[8] = {  2, -2,  1, -1,  2, -2,  1, -1 };

/* --- public function --------------------------------------------------- */
seq_t a(idx_t n)
{
    if (n == 0)
        return 0;          /* out of range, but harmless */

    /* initialise on first call */
    if (seq_len == 0) {
        cur_i = 0;
        cur_j = 0;
        visited_insert(encode_key(cur_i, cur_j));
        seq_push((seq_t)1);   /* label of (0,0) */
    }

    /* compute terms up to n */
    while (seq_len < n) {
        int best_i = -1, best_j = -1;
        seq_t best_d = (seq_t)-1;   /* large */

        for (int k = 0; k < 8; ++k) {
            int64_t ni = (int64_t)cur_i + di[k];
            int64_t nj = (int64_t)cur_j + dj[k];
            if (ni < 0 || nj < 0)
                continue;
            uint64_t key = encode_key((idx_t)ni, (idx_t)nj);
            if (visited_contains(key))
                continue;

            seq_t d = (seq_t)ni * (seq_t)ni + (seq_t)nj * (seq_t)nj;
            if (best_i == -1 ||
                d < best_d ||
                (d == best_d && (idx_t)ni < (idx_t)best_i)) {
                best_i = (int)ni;
                best_j = (int)nj;
                best_d = d;
            }
        }

        /* best move found */
        cur_i = (idx_t)best_i;
        cur_j = (idx_t)best_j;
        visited_insert(encode_key(cur_i, cur_j));

        seq_t s = (seq_t)cur_i + (seq_t)cur_j;
        seq_t lbl = (seq_t)1 + s * (s + (seq_t)1) / (seq_t)2 + (seq_t)cur_i;
        seq_push(lbl);
    }

    return seq[n - 1];
}
