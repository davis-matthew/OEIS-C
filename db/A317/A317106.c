/*
 * A317106  Squares visited by a (1,3)-leaper on a spirally numbered board and
 *          moving to the lowest available unvisited square at each step,
 *          squares labelled >=1.
 * https://oeis.org/A317106
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, fini, full
 * Formula:   not stated
 * Range:     u8 n <= 25, u16 n <= 3723, u32 n <= 3723, u64 n <= 3723,
 *            u128 n <= 3723, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A317106: squares visited by a (1,3)-leaper on a spirally numbered board,
   always moving to the lowest available unvisited square.
   The implementation simulates the walk, storing the visited squares in a
   hash table and the sequence values in a dynamic array.  Spiral numbers are
   computed on the fly from coordinates using the closed‑form mapping.  */
#include <stdlib.h>
#include <stdbool.h>
#include <stddef.h>

/* Hash table for visited coordinates ------------------------------------- */
#define VIS_HASH_SIZE   32768   /* power of two > 2 * expected number of squares  */

static int  vis_x[VIS_HASH_SIZE];
static int  vis_y[VIS_HASH_SIZE];
static unsigned char vis_used[VIS_HASH_SIZE]; /* 0 = empty, 1 = occupied */

/* Simple 2‑dimensional hash with linear probing.  */
static bool is_visited(int x, int y)
{
    unsigned long long h = ((long long)x * 73856093LL) ^ ((long long)y * 19349663LL);
    unsigned idx = (unsigned)h & (VIS_HASH_SIZE - 1);
    for (;;) {
        if (!vis_used[idx]) return false;
        if (vis_x[idx] == x && vis_y[idx] == y) return true;
        idx = (idx + 1) & (VIS_HASH_SIZE - 1);
    }
}

static void set_visited(int x, int y)
{
    unsigned long long h = ((long long)x * 73856093LL) ^ ((long long)y * 19349663LL);
    unsigned idx = (unsigned)h & (VIS_HASH_SIZE - 1);
    for (;;) {
        if (!vis_used[idx]) {
            vis_used[idx] = 1;
            vis_x[idx] = x;
            vis_y[idx] = y;
            return;
        }
        if (vis_x[idx] == x && vis_y[idx] == y) return; /* already present */
        idx = (idx + 1) & (VIS_HASH_SIZE - 1);
    }
}

/* Spiral number of a coordinate (x,y). ----------------------------------- */
static inline seq_t spiral_num(int x, int y)
{
    int ax = x >= 0 ? x : -x;
    int ay = y >= 0 ? y : -y;
    int m  = ax > ay ? ax : ay;
    seq_t M = (seq_t)m;
    seq_t n = 2 * M + 1;
    seq_t t = n * n;                     /* (2m+1)^2 */

    if (y == -m)                /* bottom side */
        return t - (M - (seq_t)x);
    else if (x == -m)           /* left side */
        return t - 2 * M - ((seq_t)y + M);
    else if (y == m)            /* top side */
        return t - 4 * M - ((seq_t)x + M);
    else                        /* right side */
        return t - 6 * M - ((seq_t)(-y) + M);
}

/* Sequence cache -------------------------------------------------------- */
static seq_t *seq_cache = NULL;
static size_t seq_cap   = 0;
static idx_t  seq_len   = 0;          /* number of terms computed so far */
static int    latest_x  = 0, latest_y = 0; /* coordinates of the last visited square */

/* Ensure seq_cache has room for at least 'needed' elements. */
static void ensure_seq_cap(size_t needed)
{
    if (needed <= seq_cap) return;
    size_t new_cap = seq_cap ? seq_cap << 1 : 8;
    while (new_cap < needed) new_cap <<= 1;
    seq_cache = (seq_t *)realloc(seq_cache, new_cap * sizeof(seq_t));
    seq_cap   = new_cap;
}

/* Main function ---------------------------------------------------------- */
seq_t a(idx_t n)
{
    if (n == 0) return 0;          /* offset is 1, caller must use n>=1 */

    if (seq_len == 0) {            /* first call – initialise */
        ensure_seq_cap(1);
        seq_cache[0] = 1;          /* center square */
        vis_used[0]   = 1;          /* mark (0,0) visited */
        vis_x[0] = 0;
        vis_y[0] = 0;
        seq_len = 1;
        latest_x = 0;
        latest_y = 0;
    }

    /* Generate terms until we have at least n of them. */
    while (seq_len < n) {
        int best_x = 0, best_y = 0;
        seq_t best_num = 0;
        bool found = false;

        /* eight (1,3)-leaper moves */
        const int dx[8] = { 1,  1, -1, -1,  3,  3, -3, -3 };
        const int dy[8] = { 3, -3,  3, -3,  1, -1,  1, -1 };

        for (int m = 0; m < 8; ++m) {
            int nx = latest_x + dx[m];
            int ny = latest_y + dy[m];
            if (is_visited(nx, ny)) continue;
            seq_t cand = spiral_num(nx, ny);
            if (!found || cand < best_num) {
                found = true;
                best_num = cand;
                best_x = nx;
                best_y = ny;
            }
        }

        if (!found) break; /* no further unvisited squares reachable */

        ensure_seq_cap(seq_len + 1);
        seq_cache[seq_len++] = best_num;
        set_visited(best_x, best_y);
        latest_x = best_x;
        latest_y = best_y;
    }

    return (n <= seq_len) ? seq_cache[n - 1] : 0;
}
