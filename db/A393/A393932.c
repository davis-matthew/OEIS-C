/*
 * A393932  Start with a point at (0,0). At each iteration, add a new point by
 *          going one step up or to the right from the current point, choosing
 *          the option that gives the least maximum number of points on a line
 *          through the new point. In case of ties, compare the number of
 *          points on the line with the next l...
 * https://oeis.org/A393932
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 87, u16 n <= 87, u32 n <= 87, u64 n <= 87, u128 n <= 87,
 *            big any n
 * Helpers:   abs_int, gcd_int
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Algorithm: a(n) is the nth step of the greedy walk on ℕ².
   Starting at (0,0), each step moves either one unit to the right
   (→  a(n)=0) or one unit up (↑  a(n)=1).  For the two candidate
   points we compute, for each, the multiset of the number of points
   lying on any line that contains the candidate.  The candidate
   with the smallest maximum line size is chosen; if tied, we compare
   the multisets lexicographically (largest to smallest), and on a
   complete tie we choose the rightward step.  This implementation
   memoises all intermediate results so that any call a(n) (n≥1) can
   be answered in O(n) time and O(n) memory for n up to several
   thousand. */

#include <stddef.h>
#include <stdint.h>
#include <stdbool.h>
#include <stdlib.h>
#include <string.h>
#include "../utils/abs_int.h"
#include "../utils/gcd_int.h"

/* ----------------------------------------------------------------------------- */
/*  Static state: the walk and auxiliary data                                */
/* ----------------------------------------------------------------------------- */
static size_t points_cap = 0;            /* capacity of points arrays          */
static idx_t *points_x = NULL;          /* x‑coordinates of the walk points   */
static idx_t *points_y = NULL;          /* y‑coordinates of the walk points   */
static seq_t *seq      = NULL;          /* a[1..max_n]                         */
static int   *freqR    = NULL;          /* frequency table for right candidate*/
static int   *freqU    = NULL;          /* frequency table for up candidate   */

static size_t cur_points = 0;            /* number of points currently stored */
static idx_t   max_n     = 0;            /* largest n for which seq is known   */

static bool    initialized = false;     /* one‑time initialisation flag       */

/* ----------------------------------------------------------------------------- */
/*  Hash map used to count directions from a candidate point                    */
/* ----------------------------------------------------------------------------- */
#define HASH_CAP (1 << 16)                /* 65536 buckets, power of two        */
static int  hash_x[HASH_CAP];
static int  hash_y[HASH_CAP];
static int  hash_cnt[HASH_CAP];
static int  hash_ver[HASH_CAP];
static int  hash_used[HASH_CAP];
static int  hash_used_cnt = 0;
static int  current_hash_ver = 1;

/* ----------------------------------------------------------------------------- */
/*  Utility functions                                                            */
/* ----------------------------------------------------------------------------- */

/* ----------------------------------------------------------------------------- */
/*  Ensure all global arrays are large enough for 'n' steps                      */
/* ----------------------------------------------------------------------------- */
static void ensure_capacity(idx_t n) {
    if (points_cap >= (size_t)(n + 1)) return;

    size_t new_cap = points_cap < 16 ? 16 : points_cap;
    while (new_cap < (size_t)(n + 1)) new_cap <<= 1;

    points_x = (idx_t*)realloc(points_x, new_cap * sizeof(idx_t));
    points_y = (idx_t*)realloc(points_y, new_cap * sizeof(idx_t));
    seq     = (seq_t*)realloc(seq    , (new_cap + 1) * sizeof(seq_t));

    freqR = (int*)realloc(freqR, (new_cap + 1) * sizeof(int));
    freqU = (int*)realloc(freqU, (new_cap + 1) * sizeof(int));

    points_cap = new_cap;
}

/* ----------------------------------------------------------------------------- */
/*  Compute frequency tables for a candidate point (cx,cy).                      */
/*  freq[1..m] will contain the number of directions that occur exactly cnt   */
/*  times (i.e., lines containing cnt existing points besides the candidate). */
/* ----------------------------------------------------------------------------- */
static void compute_freq(idx_t cx, idx_t cy,
                         size_t m,
                         int   *freq,
                         int   *max_out) {
    int ver = ++current_hash_ver;
    hash_used_cnt = 0;

    /* zero frequency array */
    memset(freq, 0, (m + 1) * sizeof(int));

    for (size_t i = 0; i < m; ++i) {
        int dx = (int)(points_x[i]) - (int)(cx);
        int dy = (int)(points_y[i]) - (int)(cy);

        int adx = abs_int(dx), ady = abs_int(dy);
        int g   = gcd_int(adx, ady);
        if (g == 0) continue;              /* identical point (should not happen) */

        dx /= g;  dy /= g;
        if (dx < 0 || (dx == 0 && dy < 0)) {
            dx = -dx; dy = -dy;            /* canonical orientation */
        }

        /* simple hash key */
        unsigned int h = ((uint32_t)dx << 16) ^ (uint32_t)dy;
        int idx = h & (HASH_CAP - 1);
        while (1) {
            if (hash_ver[idx] != ver) {    /* empty bucket */
                hash_ver[idx] = ver;
                hash_x[idx]   = dx;
                hash_y[idx]   = dy;
                hash_cnt[idx] = 1;
                hash_used[hash_used_cnt++] = idx;
                break;
            }
            if (hash_x[idx] == dx && hash_y[idx] == dy) {
                hash_cnt[idx]++;
                break;
            }
            idx = (idx + 1) & (HASH_CAP - 1);
        }
    }

    int maxCnt = 0;
    for (int k = 0; k < hash_used_cnt; ++k) {
        int idx = hash_used[k];
        int cnt = hash_cnt[idx];
        if (cnt > maxCnt) maxCnt = cnt;
        freq[cnt]++;
    }
    *max_out = maxCnt;
}

/* ----------------------------------------------------------------------------- */
/*  Sequence accessor.  Returns a(n) for n >= 1.                                 */
/* ----------------------------------------------------------------------------- */
seq_t a(idx_t n) {
    if (n < 1) return 0;                /* undefined for n=0 – return 0 safely */

    /* initialise static state on first call */
    if (!initialized) {
        initialized = true;
        ensure_capacity(16);            /* small starter capacity */
        points_x[0] = 0;
        points_y[0] = 0;
        cur_points  = 1;
        max_n       = 0;
    }

    /* generate terms up to n if necessary */
    if (n > max_n) {
        ensure_capacity(n + 1);
        for (idx_t step = max_n + 1; step <= n; ++step) {
            size_t m = cur_points;                /* existing points count */
            idx_t last_x = points_x[cur_points - 1];
            idx_t last_y = points_y[cur_points - 1];

            /* candidates */
            idx_t right_x = last_x + 1, right_y = last_y; /* move right */
            idx_t up_x    = last_x,     up_y    = last_y + 1; /* move up */

            /* frequency tables */
            int maxR, maxU;
            compute_freq(right_x, right_y, m, freqR, &maxR);
            compute_freq(up_x,    up_y,    m, freqU, &maxU);

            /* lexicographic comparison of frequency tables */
            int choice = 0;                    /* 0 – right, 1 – up */
            int maxComp = maxR > maxU ? maxR : maxU;
            for (int cnt = maxComp; cnt >= 1; --cnt) {
                int fr = (cnt <= maxR) ? freqR[cnt] : 0;
                int fu = (cnt <= maxU) ? freqU[cnt] : 0;
                if (fr < fu) { choice = 0; break; }
                if (fu < fr) { choice = 1; break; }
            }
            /* on complete tie we keep choice = 0 (right) */

            seq[step] = (seq_t)choice;

            /* append chosen point to walk */
            if (choice == 0) {
                points_x[cur_points] = right_x;
                points_y[cur_points] = right_y;
            } else {
                points_x[cur_points] = up_x;
                points_y[cur_points] = up_y;
            }
            ++cur_points;
        }
        max_n = n;
    }

    return seq[n];
}
