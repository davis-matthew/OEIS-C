/*
 * A400911  Squares visited by a chess king moving on the square spiral,
 *          always moving to an adjacent unvisited square whose number has the
 *          largest sum of digits; ties are resolved by choosing the lowest
 *          number.
 * https://oeis.org/A400911
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base, walk, fini, full, new
 * Formula:   not stated
 * Range:     u8 n <= 46, u16 n <= 2032, u32 n <= 2032, u64 n <= 2032,
 *            u128 n <= 2032, big any n
 * Helpers:   coord_to_num, digit_sum_int
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computes OEIS sequence A400911: squares visited by a chess king
   walking on the square spiral, always moving to an
   unvisited adjacent square with the largest digit sum
   (ties broken by the lowest number). */
#include <stdbool.h>
#include <stddef.h>
#include "../utils/coord_to_num.h"
#include "../utils/digit_sum_int.h"

/* Helper to compute the integer sqrt of a seq_t (fits in unsigned long long). */
static inline seq_t isqrt_u64(seq_t v) {
    unsigned long long r = 0;
    while ((r + 1) * (r + 1) <= v) r++;
    return (seq_t)r;
}

/* Inverse mapping: from square number n to its (x,y) coordinate. */
static inline void number_to_coord(seq_t n, int *x, int *y) {
    if (n == 1) { *x = 0; *y = 0; return; }
    seq_t r = isqrt_u64(n - 1) + 1;          /* ceil sqrt(n) */
    if (((unsigned long long)r & 1ULL) == 0ULL) r++;   /* make it odd */
    int L = (int)((r - 1) / 2);                /* layer */

    unsigned long long prev_max = (unsigned long long)(2 * L - 1);
    prev_max = prev_max * prev_max;
    seq_t t = n - (seq_t)prev_max - 1;         /* offset on the layer (0‑based) */

    int side = (int)(t / (2 * L));             /* 0:right,1:top,2:left,3:bottom */
    int offset = (int)(t % (2 * L));           /* steps along the side */

    switch (side) {
        case 0: /* right side, moving up */
            *x =  L;
            *y = -L + 1 + offset;
            break;
        case 1: /* top side, moving left */
            *x =  L - 1 - offset;
            *y =  L;
            break;
        case 2: /* left side, moving down */
            *x = -L;
            *y =  L - 1 - offset;
            break;
        case 3: /* bottom side, moving right */
            *x = -L + 1 + offset;
            *y = -L;
            break;
    }
}

/* Sequence generation constants. */
#define SEQ_LEN_MAX 2032   /* the sequence ends at a(2032) */

/* Static storage for memoisation. */
static seq_t seq[SEQ_LEN_MAX + 1];
static size_t seq_len = 0;
static seq_t visited_nums[SEQ_LEN_MAX];
static size_t visited_len = 0;
static bool initialized = false;

/* Main function required by the benchmark. */
seq_t a(idx_t n) {
    if (n == 0) return 0;          /* offset 1 – undefined for n=0 */

    if (!initialized) {
        /* build the sequence once */
        seq[1] = (seq_t)1;
        seq_len = 1;
        visited_nums[0] = (seq_t)1;
        visited_len = 1;

        seq_t cur = 1;
        for (;;) {
            int cx, cy;
            number_to_coord(cur, &cx, &cy);

            seq_t best_num = 0;
            int best_dsum = -1;

            for (int dx = -1; dx <= 1; ++dx) {
                for (int dy = -1; dy <= 1; ++dy) {
                    if (dx == 0 && dy == 0) continue;
                    int nx = cx + dx;
                    int ny = cy + dy;
                    int nnum = coord_to_num(nx, ny);
                    if (nnum <= 0) continue;

                    /* check if already visited */
                    bool seen = false;
                    for (size_t i = 0; i < visited_len; ++i) {
                        if (visited_nums[i] == (seq_t)nnum) {
                            seen = true;
                            break;
                        }
                    }
                    if (seen) continue;

                    int dsum = digit_sum_int((unsigned long long)nnum);
                    if (dsum > best_dsum || (dsum == best_dsum && nnum < (int)best_num)) {
                        best_dsum = dsum;
                        best_num = (seq_t)nnum;
                    }
                }
            }

            if (best_num == 0) break;  /* no unvisited neighbour */

            cur = best_num;
            visited_nums[visited_len++] = cur;
            seq[++seq_len] = cur;
        }
        initialized = true;
    }

    if ((size_t)n > seq_len) return 0;  /* sequence is finite */
    return seq[n];
}
