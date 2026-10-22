/*
 * A335899  Squares visited when moving on a square-spiral numbered board to
 *          an unvisited diagonally adjacent square containing the spiral
 *          number with the fewest divisors. In case of a tie it chooses the
 *          square with the lowest spiral number.
 * https://oeis.org/A335899
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, walk, fini, full
 * Formula:   a(n) is the n‑th visited square in the A335899 walk. The walk
 *            starts at the centre of an infinite Ulam spiral. At each step it
 *            moves to an unvisited diagonally adjacent square whose spiral
 *            number has the fewest divisors; ties are broken by choosing the
 *            smallest spiral number. The sequence stops w...
 * Range:     u8 n <= 28, u16 n <= 28, u32 n <= 28, u64 n <= 28, u128 n <= 28,
 *            big any n
 * Helpers:   spiral_number_from
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>
#include "../utils/spiral_number_from.h"

/* Basic divisor count for 64‑bit numbers. */
static inline uint64_t divisor_count_u64(uint64_t n)
{
    uint64_t cnt = 1;
    for (uint64_t p = 2; p * p <= n; ++p) {
        if (n % p == 0) {
            uint64_t exp = 0;
            while (n % p == 0) {
                n /= p;
                ++exp;
            }
            cnt *= (exp + 1);
        }
    }
    if (n > 1) cnt *= 2;
    return cnt;
}

typedef struct {
    int64_t x, y;
    seq_t   num;
} node_t;

static node_t *nodes      = NULL;
static size_t  nodes_cap  = 0;
static size_t  nodes_len  = 0;
static bool    finished   = false;

/* Ensure the nodes array can hold 'needed' elements. */
static void ensure_nodes_capacity(size_t needed)
{
    if (nodes_cap >= needed) return;
    size_t new_cap = nodes_cap ? nodes_cap * 2 : 8;
    if (new_cap < needed) new_cap = needed;
    node_t *tmp = (node_t *)realloc(nodes, new_cap * sizeof(node_t));
    if (!tmp) abort();
    nodes = tmp;
    nodes_cap = new_cap;
}

seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0;   /* below offset – not used */

    if (nodes_len == 0) {  /* first call – initialise centre */
        ensure_nodes_capacity(1);
        nodes[0].x = 0;
        nodes[0].y = 0;
        nodes[0].num = (seq_t)1;
        nodes_len = 1;
    }

    /* If we already know the sequence has finished, skip the walk. */
    if (!finished && nodes_len < (size_t)n) {
        static const int8_t dx[4] = { 1,  1, -1, -1 };
        static const int8_t dy[4] = { 1, -1,  1, -1 };

        while (nodes_len < (size_t)n) {
            int64_t cx = nodes[nodes_len - 1].x;
            int64_t cy = nodes[nodes_len - 1].y;
            bool found = false;
            uint64_t best_div = ~0ULL;
            uint64_t best_num  = 0;
            int64_t  best_x    = 0;
            int64_t  best_y    = 0;

            for (int i = 0; i < 4; ++i) {
                int64_t nx = cx + dx[i];
                int64_t ny = cy + dy[i];
                seq_t   val = spiral_number_from(nx, ny);

                /* check if already visited */
                bool vis = false;
                for (size_t j = 0; j < nodes_len; ++j) {
                    if (nodes[j].num == val) { vis = true; break; }
                }
                if (vis) continue;

                uint64_t d = divisor_count_u64((uint64_t)val);
                if (!found || d < best_div || (d == best_div && (uint64_t)val < best_num)) {
                    found = true;
                    best_div = d;
                    best_num = (uint64_t)val;
                    best_x = nx;
                    best_y = ny;
                }
            }

            if (!found) { /* no legal move – sequence ends */
                finished = true;
                break;
            }

            ensure_nodes_capacity(nodes_len + 1);
            nodes[nodes_len].x   = best_x;
            nodes[nodes_len].y   = best_y;
            nodes[nodes_len].num = (seq_t)best_num;
            ++nodes_len;
        }
    }

    if (n <= nodes_len) return nodes[n - 1].num;
    return (seq_t)0;   /* sequence finished before reaching n */
}
