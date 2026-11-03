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
 * Validated: partial, pass, 60 terms through n = 60 (2026-11-01)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Walk: generate the sequence by simulating the king’s moves on the
   spiral‑numbered board until the king gets stuck.  The function
   returns a(1)=1, a(2)=2, … for 1 ≤ n ≤ 511.  For larger n it returns 0. */
#include <stdbool.h>

/* Convert (x,y) coordinates of the spiral board to the corresponding
   spiral number.  Both x and y are signed 64‑bit integers. */
static inline seq_t number_from_coord(long long x, long long y) {
    if (x == 0 && y == 0) return (seq_t)1;

    idx_t ax = x >= 0 ? (idx_t)x : (idx_t)(-x);
    idx_t ay = y >= 0 ? (idx_t)y : (idx_t)(-y);
    idx_t k  = ax > ay ? ax : ay;
    idx_t side_len = 2 * k;
    seq_t side_len_seq = (seq_t)side_len;

    seq_t max_val = ((seq_t)(2 * k + 1)) * ((seq_t)(2 * k + 1));
    seq_t ring_start = max_val - (seq_t)(8 * k) + 1;
    seq_t val;

    long long kk = (long long)k;
    if (x == kk && y >= -kk + 1 && y <= kk) {
        seq_t offset = (seq_t)(y + kk - 1);
        val = ring_start + offset;
    } else if (y == kk && x >= -kk && x <= kk - 1) {
        seq_t offset = side_len_seq + ((seq_t)(kk - 1 - x));
        val = ring_start + offset;
    } else if (x == -kk && y >= -kk && y <= kk - 1) {
        seq_t offset = 2 * side_len_seq + ((seq_t)(kk - 1 - y));
        val = ring_start + offset;
    } else if (y == -kk && x >= -kk + 1 && x <= kk) {
        seq_t offset = 3 * side_len_seq + ((seq_t)(x + kk - 1));
        val = ring_start + offset;
    } else {
        /* should not happen */
        val = max_val;
    }
    return val;
}

/* Convert a spiral number to its (x,y) coordinates.  The result fits
   easily in signed 64‑bit integers. */
static inline void number_to_coord(seq_t n, long long *x, long long *y) {
    if (n == (seq_t)1) { *x = 0; *y = 0; return; }

    idx_t k = 0;
    while (true) {
        seq_t side = (seq_t)(2 * k + 1);
        seq_t side_sq = side * side;
        if (side_sq >= n) break;
        ++k;
    }

    idx_t side_len = 2 * k;
    seq_t max_val = ((seq_t)(2 * k + 1)) * ((seq_t)(2 * k + 1));
    seq_t ring_start = max_val - (seq_t)(8 * k) + 1;
    seq_t offset_seq = n - ring_start;
    idx_t off = (idx_t)offset_seq;
    long long kk = (long long)k;

    if (off < side_len) {
        *x = kk;
        *y = -kk + 1 + (long long)off;
    } else if (off < side_len * 2) {
        idx_t t = off - side_len;
        *x = kk - 1 - (long long)t;
        *y = kk;
    } else if (off < side_len * 3) {
        idx_t t = off - 2 * side_len;
        *x = -kk;
        *y = kk - 1 - (long long)t;
    } else {
        idx_t t = off - 3 * side_len;
        *x = -kk + 1 + (long long)t;
        *y = -kk;
    }
}

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
