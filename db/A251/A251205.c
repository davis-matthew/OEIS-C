/*
 * A251205  Number of (n+1)X(3+1) 0..2 arrays with every 2X2 subblock summing
 *          to a nonzero multiple of 2.
 * https://oeis.org/A251205
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = number of (n+1)×4 arrays over {0,1,2} whose every * 2×2
 *            subblock sums to a non‑zero even number. * Computation via
 *            transfer matrix: * • All 81 possible 4‑tuples form the state
 *            set. * • A 0/1 adjacency matrix M encodes compatibility of
 *            consecutive rows. * • a(n) = 1ᵀ Mⁿ 1 (walks of length n...
 * Range:     u8 none, u16 n <= 2, u32 n <= 7, u64 n <= 15, u128 n <= 31,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

static bool init_done = false;
static unsigned char M[81][81];
static seq_t cur[81], next_arr[81];
static seq_t *totals = NULL;
static size_t totals_size = 0;
static size_t totals_cap   = 0;

/* Build the adjacency matrix once. */
static void init(void)
{
    int state[81][4];
    for (int i = 0; i < 81; ++i) {
        int t = i;
        state[i][0] = t / 27 % 3;
        state[i][1] = t / 9  % 3;
        state[i][2] = t / 3  % 3;
        state[i][3] = t % 3;
    }

    for (int i = 0; i < 81; ++i) {
        for (int j = 0; j < 81; ++j) {
            bool ok = true;
            for (int c = 0; c < 3; ++c) {
                int sum = state[i][c] + state[i][c+1] +
                          state[j][c] + state[j][c+1];
                if ((sum & 1) != 0 || sum == 0) { ok = false; break; }
            }
            M[i][j] = ok ? 1 : 0;
        }
    }

    for (int i = 0; i < 81; ++i) cur[i] = (seq_t)1;
    init_done = true;
}

/* Ensure the cache can hold ‘needed’ entries. */
static void grow_totals(size_t needed)
{
    if (totals_cap >= needed) return;
    size_t new_cap = totals_cap ? totals_cap : 8;
    while (new_cap < needed) new_cap <<= 1;
    seq_t *tmp = (seq_t*)realloc(totals, new_cap * sizeof(seq_t));
    if (!tmp) abort();
    totals = tmp;
    totals_cap = new_cap;
}

seq_t a(idx_t n)
{
    if (!init_done) init();

    while (totals_size < (size_t)n) {
        memset(next_arr, 0, sizeof next_arr);

        /* next_arr[j] = Σ_i cur[i] * M[i][j] */
        for (int i = 0; i < 81; ++i) {
            seq_t val = cur[i];
            if (val == 0) continue;
            for (int j = 0; j < 81; ++j) {
                if (M[i][j]) next_arr[j] += val;
            }
        }

        seq_t sum = 0;
        for (int j = 0; j < 81; ++j) sum += next_arr[j];

        grow_totals(totals_size + 1);
        totals[totals_size] = sum;
        ++totals_size;

        memcpy(cur, next_arr, sizeof cur);
    }

    return totals[n - 1];
}
