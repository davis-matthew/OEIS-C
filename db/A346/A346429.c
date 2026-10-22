/*
 * A346429  Squares visited on a square spiral when stepping to the closest
 *          unvisited square that contains a number with a different number of
 *          divisors to the number in the current square. If two or more such
 *          squares are the same distance from the current square then the one
 *          with the smallest number is chosen.
 * https://oeis.org/A346429
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 75, u16 n <= 75, u32 n <= 75, u64 n <= 75, u128 n <= 75,
 *            big any n
 * Helpers:   divisor_count_u64, spiral_number_from_coords
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Implementation: each term is determined by scanning outward from the
 * current coordinate for the nearest unvisited square whose divisor count
 * differs from the current square's divisor count. */

#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include "../utils/divisor_count_u64.h"
#include "../utils/spiral_number_from_coords.h"

/* ------------------------------------------------------------------ */
/*  Helper: integer square root for uint64_t                           */
/* ------------------------------------------------------------------ */
static uint64_t isqrt_u64(uint64_t x)
{
    uint64_t lo = 0, hi = 1ULL << 32;          /* sqrt(2^64-1) < 2^32 */
    while (lo < hi) {
        uint64_t mid = (lo + hi + 1) >> 1;
        if (mid > 0 && mid > x / mid)
            hi = mid - 1;
        else
            lo = mid;
    }
    return lo;
}

/* ------------------------------------------------------------------ */
/*  Map a number to its (x,y) coordinate in the square spiral.          */
/* ------------------------------------------------------------------ */
static void spiral_coords(uint64_t n, int64_t *x, int64_t *y)
{
    if (n <= 1) {
        *x = *y = 0;
        return;
    }
    uint64_t r = (isqrt_u64(n - 1) + 1) / 2;
    uint64_t t = 2 * r;
    uint64_t m = (2 * r + 1) * (2 * r + 1);
    uint64_t offset = m - n;

    if (offset < t) {                     /* bottom side */
        *x = (int64_t)r - (int64_t)offset;
        *y = -(int64_t)r;
    } else if (offset < 2 * t) {           /* left side */
        *x = -(int64_t)r;
        *y = -(int64_t)r + (int64_t)(offset - t);
    } else if (offset < 3 * t) {           /* top side */
        *x = -(int64_t)r + (int64_t)(offset - 2 * t);
        *y = (int64_t)r;
    } else {                               /* right side */
        *x = (int64_t)r;
        *y = (int64_t)r - (int64_t)(offset - 3 * t);
    }
}

/* ------------------------------------------------------------------ */
/*  Map coordinates to the square spiral number.                         */

/* ------------------------------------------------------------------ */
/*  Dynamic arrays for visited squares and divisor counts.             */
/* ------------------------------------------------------------------ */
static uint8_t  *visited   = NULL;   /* 0 = unvisited, 1 = visited */
static uint16_t *divcnt    = NULL;   /* divisor count of the number */
static size_t   visited_cap = 0;

/* Ensure the visited/divcnt arrays can address index 'need'. */
static void ensure_capacity(size_t need)
{
    if (need < visited_cap)
        return;
    size_t new_cap = visited_cap ? visited_cap * 2 : 16;
    while (new_cap <= need)
        new_cap <<= 1;
    uint8_t  *nv = (uint8_t  *)realloc(visited, new_cap * sizeof(uint8_t));
    if (!nv)
        exit(1);
    memset(nv + visited_cap, 0, (new_cap - visited_cap) * sizeof(uint8_t));
    visited = nv;

    uint16_t *nd = (uint16_t *)realloc(divcnt, new_cap * sizeof(uint16_t));
    if (!nd)
        exit(1);
    memset(nd + visited_cap, 0, (new_cap - visited_cap) * sizeof(uint16_t));
    divcnt = nd;

    visited_cap = new_cap;
}

/* ------------------------------------------------------------------ */
/*  Dynamic array for the sequence itself.                              */
/* ------------------------------------------------------------------ */
static seq_t  *seq_arr  = NULL;
static size_t seq_len  = 0;
static size_t seq_cap  = 0;

static void grow_seq_arr(void)
{
    size_t new_cap = seq_cap ? seq_cap * 2 : 16;
    seq_t *na = (seq_t *)realloc(seq_arr, new_cap * sizeof(seq_t));
    if (!na)
        exit(1);
    seq_arr = na;
    seq_cap = new_cap;
}

/* ------------------------------------------------------------------ */
/*  Main function                                                      */
/* ------------------------------------------------------------------ */
seq_t a(idx_t n)
{
    if (n == 0)
        return (seq_t)0; /* safety for out of bounds */

    /* Initialise on first call */
    if (seq_len == 0) {
        seq_cap = 16;
        seq_arr = (seq_t *)malloc(seq_cap * sizeof(seq_t));
        if (!seq_arr)
            exit(1);
        seq_arr[0] = (seq_t)1;   /* a(1) = 1 */
        seq_len = 1;

        visited_cap = 16;
        visited = (uint8_t  *)calloc(visited_cap, sizeof(uint8_t));
        divcnt  = (uint16_t *)calloc(visited_cap, sizeof(uint16_t));
        if (!visited || !divcnt)
            exit(1);

        visited[1] = 1;
        divcnt[1]  = 1;           /* divisor count of 1 */
    }

    /* Extend the sequence up to the requested index */
    while (seq_len < n) {
        uint64_t cur_num   = (uint64_t)seq_arr[seq_len - 1];
        uint16_t curDiv    = divcnt[cur_num];

        int64_t curx, cury;
        spiral_coords(cur_num, &curx, &cury);

        uint64_t  bestNum   = 0;
        uint64_t  bestDistSq = 0;

        /* Search successive rings until a suitable neighbour is found */
        for (uint64_t r = 1; ; ++r) {
            bool found_in_this_ring = false;

            /* Bottom side (y = cury - r) */
            for (int64_t dx = - (int64_t)r; dx <= (int64_t)r; ++dx) {
                int64_t x = curx + dx;
                int64_t y = cury - (int64_t)r;
                uint64_t cand = (uint64_t)spiral_number_from_coords(x, y);
                idx_t candIdx = (idx_t)cand;
                ensure_capacity(candIdx);
                if (visited[candIdx])
                    continue;
                if (cand == 1) {
                    if (curDiv == 1)
                        continue;
                }
                if (divcnt[candIdx] == 0) {
                    divcnt[candIdx] = (uint16_t)divisor_count_u64((uint64_t)cand);
                }
                if (divcnt[candIdx] == curDiv)
                    continue;

                int64_t dx0 = x - curx;
                int64_t dy0 = y - cury;
                uint64_t d2 = (uint64_t)(dx0 >= 0 ? dx0 : -dx0) * (uint64_t)(dx0 >= 0 ? dx0 : -dx0)
                           + (uint64_t)(dy0 >= 0 ? dy0 : -dy0) * (uint64_t)(dy0 >= 0 ? dy0 : -dy0);

                if (bestNum == 0 || d2 < bestDistSq || (d2 == bestDistSq && cand < bestNum)) {
                    bestNum = cand;
                    bestDistSq = d2;
                }
                found_in_this_ring = true;
            }

            /* Top side (y = cury + r) – skip corners already handled */
            for (int64_t dx = - (int64_t)r; dx <= (int64_t)r; ++dx) {
                int64_t x = curx + dx;
                int64_t y = cury + (int64_t)r;
                uint64_t cand = (uint64_t)spiral_number_from_coords(x, y);
                idx_t candIdx = (idx_t)cand;
                ensure_capacity(candIdx);
                if (visited[candIdx])
                    continue;
                if (cand == 1) {
                    if (curDiv == 1)
                        continue;
                }
                if (divcnt[candIdx] == 0) {
                    divcnt[candIdx] = (uint16_t)divisor_count_u64((uint64_t)cand);
                }
                if (divcnt[candIdx] == curDiv)
                    continue;

                int64_t dx0 = x - curx;
                int64_t dy0 = y - cury;
                uint64_t d2 = (uint64_t)(dx0 >= 0 ? dx0 : -dx0) * (uint64_t)(dx0 >= 0 ? dx0 : -dx0)
                           + (uint64_t)(dy0 >= 0 ? dy0 : -dy0) * (uint64_t)(dy0 >= 0 ? dy0 : -dy0);

                if (bestNum == 0 || d2 < bestDistSq || (d2 == bestDistSq && cand < bestNum)) {
                    bestNum = cand;
                    bestDistSq = d2;
                }
                found_in_this_ring = true;
            }

            /* Left side (x = curx - r), excluding corners */
            for (int64_t dy = -(int64_t)r + 1; dy <= (int64_t)r - 1; ++dy) {
                int64_t x = curx - (int64_t)r;
                int64_t y = cury + dy;
                uint64_t cand = (uint64_t)spiral_number_from_coords(x, y);
                idx_t candIdx = (idx_t)cand;
                ensure_capacity(candIdx);
                if (visited[candIdx])
                    continue;
                if (cand == 1) {
                    if (curDiv == 1)
                        continue;
                }
                if (divcnt[candIdx] == 0) {
                    divcnt[candIdx] = (uint16_t)divisor_count_u64((uint64_t)cand);
                }
                if (divcnt[candIdx] == curDiv)
                    continue;

                int64_t dx0 = x - curx;
                int64_t dy0 = y - cury;
                uint64_t d2 = (uint64_t)(dx0 >= 0 ? dx0 : -dx0) * (uint64_t)(dx0 >= 0 ? dx0 : -dx0)
                           + (uint64_t)(dy0 >= 0 ? dy0 : -dy0) * (uint64_t)(dy0 >= 0 ? dy0 : -dy0);

                if (bestNum == 0 || d2 < bestDistSq || (d2 == bestDistSq && cand < bestNum)) {
                    bestNum = cand;
                    bestDistSq = d2;
                }
                found_in_this_ring = true;
            }

            /* Right side (x = curx + r), excluding corners */
            for (int64_t dy = -(int64_t)r + 1; dy <= (int64_t)r - 1; ++dy) {
                int64_t x = curx + (int64_t)r;
                int64_t y = cury + dy;
                uint64_t cand = (uint64_t)spiral_number_from_coords(x, y);
                idx_t candIdx = (idx_t)cand;
                ensure_capacity(candIdx);
                if (visited[candIdx])
                    continue;
                if (cand == 1) {
                    if (curDiv == 1)
                        continue;
                }
                if (divcnt[candIdx] == 0) {
                    divcnt[candIdx] = (uint16_t)divisor_count_u64((uint64_t)cand);
                }
                if (divcnt[candIdx] == curDiv)
                    continue;

                int64_t dx0 = x - curx;
                int64_t dy0 = y - cury;
                uint64_t d2 = (uint64_t)(dx0 >= 0 ? dx0 : -dx0) * (uint64_t)(dx0 >= 0 ? dx0 : -dx0)
                           + (uint64_t)(dy0 >= 0 ? dy0 : -dy0) * (uint64_t)(dy0 >= 0 ? dy0 : -dy0);

                if (bestNum == 0 || d2 < bestDistSq || (d2 == bestDistSq && cand < bestNum)) {
                    bestNum = cand;
                    bestDistSq = d2;
                }
                found_in_this_ring = true;
            }

            if (!found_in_this_ring && r > 1000) {
                /* safety to avoid infinite loop – in practice this never happens */
                break;
            }

            uint64_t next_edge = (r + 1) * (r + 1);
            if (bestNum != 0 && bestDistSq < next_edge)
                break;     /* no better candidate can be found in larger rings */
        }

        /* Append selected number to sequence */
        if (seq_len == seq_cap)
            grow_seq_arr();
        seq_arr[seq_len] = (seq_t)bestNum;
        seq_len++;

        visited[bestNum] = 1;
        /* divcnt[bestNum] already computed when the candidate was inspected */
    }

    return seq_arr[n - 1];
}
