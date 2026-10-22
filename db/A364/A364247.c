/*
 * A364247  Squares visited by the chess king on a spiral-numbered board,
 *          where the king moves to the square with the fewest steps to reach
 *          1 using the 3x+1 function. In case of a tie, the king moves to the
 *          square with the smallest number.
 * https://oeis.org/A364247
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, walk, fini
 * Formula:   not stated
 * Range:     u8 none, u16 n <= 60, u32 n <= 60, u64 n <= 60, u128 n <= 60,
 *            big any n
 * Helpers:   number_from_coord, number_to_coord
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Walk: generate the sequence by simulating the king’s moves on the
   spiral‑numbered board until the king gets stuck.  The function
   returns a(1)=1, a(2)=2, … for 1 ≤ n ≤ 511.  For larger n it returns 0. */
#include <stdbool.h>
#include "../utils/number_from_coord.h"
#include "../utils/number_to_coord.h"

static inline idx_t collatz_steps(seq_t n) {
    idx_t steps = 0;
    while (n != (seq_t)1) {
        if (n & (seq_t)1) n = n * 3 + 1;
        else             n >>= 1;
        ++steps;
    }
    return steps;
}

/* Pre‑computed sequence values (maximum 512). */
static seq_t seq_values[512];
static idx_t seq_len = 0;
static bool initialized = false;

/* Build the entire walk once. */
static void init_sequence(void) {
    seq_values[0] = (seq_t)1;
    seq_len = 1;

    while (seq_len < 512) {
        seq_t cur = seq_values[seq_len - 1];
        long long cx, cy;
        number_to_coord(cur, &cx, &cy);

        seq_t best = 0;
        idx_t best_steps = (idx_t)-1;   /* largest possible value */

        for (long long dx = -1; dx <= 1; ++dx) {
            for (long long dy = -1; dy <= 1; ++dy) {
                if (dx == 0 && dy == 0) continue;
                long long nx = cx + dx;
                long long ny = cy + dy;
                seq_t neigh = number_from_coord(nx, ny);

                /* check if already visited */
                bool visited = false;
                for (idx_t i = 0; i < seq_len; ++i) {
                    if (seq_values[i] == neigh) { visited = true; break; }
                }
                if (visited) continue;

                idx_t steps = collatz_steps(neigh);
                if (steps < best_steps) {
                    best_steps = steps;
                    best = neigh;
                } else if (steps == best_steps && neigh < best) {
                    best = neigh;
                }
            }
        }

        if (best == 0) break;            /* no legal move */
        seq_values[seq_len++] = best;
    }
    initialized = true;
}

seq_t a(idx_t n) {
    if (!initialized) init_sequence();
    if (n == 0 || n > seq_len) return (seq_t)0;
    return seq_values[n - 1];
}
