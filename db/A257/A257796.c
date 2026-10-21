/*
 * A257796  Smallest value of the loop in which n ends, when iterating the map
 *          (A257588) which sends a number to absolute value of first digit
 *          squared minus second digit squared plus third digit squared etc.
 * https://oeis.org/A257796
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 n <= 999, u16 n <= 999, u32 n <= 999, u64 n <= 999,
 *            u128 n <= 999, big any n
 * Helpers:   map_from_uint64
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* A257796 – Compute the smallest value in the loop reached by the
   alternating‑digit‑square map.  The algorithm precomputes all cycles
   for numbers up to 1620 (the maximum possible value of the map), stores
   their minimal element, and then looks up the result for any N. */

#include <stdint.h>
#include <stdbool.h>
#include "../utils/map_from_uint64.h"

/* maximum value that the map can produce for any 64‑bit integer */
#define MAX_MAP 1620          /* 81 * 20 digits */

/* forward declarations */
static inline seq_t map_from_uint64(uint64_t v);
static void init_precompute(void);

/* storage for the minimal loop values */
static seq_t loop_min[MAX_MAP + 1];
static bool  computed[MAX_MAP + 1];
static int   map_arr[MAX_MAP + 1];
static bool  initialized = false;

/* precompute all cycle minima for numbers 0 .. MAX_MAP */
static void init_precompute(void)
{
    /* compute the map for all numbers */
    for (int i = 0; i <= MAX_MAP; ++i) {
        map_arr[i] = (int)map_from_uint64((uint64_t)i);
        computed[i] = false;
    }

    int pos[MAX_MAP + 1];
    int path[MAX_MAP + 1];

    for (int start = 0; start <= MAX_MAP; ++start) {
        if (computed[start]) continue;

        /* reset position markers for this traversal */
        for (int i = 0; i <= MAX_MAP; ++i) pos[i] = -1;
        int path_len = 0;
        int cur = start;

        while (1) {
            if (computed[cur]) {
                /* path leads to an already known cycle minimum */
                seq_t val = loop_min[cur];
                for (int j = 0; j < path_len; ++j) {
                    loop_min[path[j]] = val;
                    computed[path[j]] = true;
                }
                break;
            }
            if (pos[cur] != -1) {
                /* a cycle has been found */
                int cycle_start = pos[cur];
                seq_t cycle_min = path[cycle_start];
                for (int j = cycle_start + 1; j < path_len; ++j) {
                    if (path[j] < cycle_min) cycle_min = path[j];
                }
                /* assign cycle minimum to all nodes in the cycle */
                for (int j = cycle_start; j < path_len; ++j) {
                    loop_min[path[j]] = cycle_min;
                    computed[path[j]] = true;
                }
                /* all preceding nodes also lead to the same cycle */
                for (int j = 0; j < cycle_start; ++j) {
                    loop_min[path[j]] = cycle_min;
                    computed[path[j]] = true;
                }
                break;
            }

            /* new node in the current path */
            pos[cur] = path_len;
            path[path_len++] = cur;
            cur = map_arr[cur];
        }
    }

    initialized = true;
}

/* return the a(n) value of OEIS A257796 */
seq_t a(idx_t n)
{
    if (!initialized) init_precompute();

    if (n <= (idx_t)MAX_MAP) {
        return loop_min[(int)n];
    } else {
        seq_t m = map_from_uint64((uint64_t)n);
        return loop_min[(int)m];
    }
}
