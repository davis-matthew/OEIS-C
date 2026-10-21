/*
 * A281885  Number of nX5 0..1 arrays with no element equal to more than four
 *          of its king-move neighbors, with the exception of exactly one
 *          element, and with new values introduced in order 0 sequentially
 *          upwards.
 * https://oeis.org/A281885
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 2, u16 n <= 3, u32 n <= 7, u64 n <= 15, u128 n <= 31,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* a(n) = number of binary n×5 arrays having exactly one
   element that equals more than four of its king‑move neighbors.
   Computed by dynamic programming over rows: each state keeps
   the previous row, the current row and whether an exception
   has already been used.  The result is divided by 2 to
   correct a double‑counting inherent in the construction. */
#include <stdint.h>
#include <stddef.h>
#include <stdbool.h>

/* constants for rows */
#define ROWS 32           /* 2^5 possible rows */
#define NO_ROW 32         /* sentinel value for “no row” */

/* transition masks */
static uint32_t zero_mask[33][32];
static uint32_t one_mask[33][32];
static uint8_t final_bits[33][32];

/* count violating cells in row mid, given neighbors prev and next
   (use NO_ROW when a neighbor does not exist). */
static uint8_t compute_bits(int prev_idx, uint8_t mid, int next_idx)
{
    const bool up_exists   = (prev_idx != NO_ROW);
    const bool down_exists = (next_idx != NO_ROW);
    const uint8_t up   = up_exists   ? (uint8_t)prev_idx : 0;
    const uint8_t down = down_exists ? (uint8_t)next_idx : 0;
    uint8_t bits = 0;

    for (int j = 0; j < 5; ++j) {
        const uint8_t midbit = (mid >> j) & 1;
        int neigh = 0;

        /* same row neighbors */
        if (j > 0     && ((mid >> (j-1)) & 1) == midbit) ++neigh;
        if (j < 4     && ((mid >> (j+1)) & 1) == midbit) ++neigh;

        /* upper row neighbors */
        if (up_exists && ((up   >> j)     & 1) == midbit) ++neigh;
        if (up_exists && j > 0             && ((up   >> (j-1)) & 1) == midbit) ++neigh;
        if (up_exists && j < 4             && ((up   >> (j+1)) & 1) == midbit) ++neigh;

        /* lower row neighbors */
        if (down_exists && ((down >> j) & 1) == midbit) ++neigh;
        if (down_exists && j > 0            && ((down >> (j-1)) & 1) == midbit) ++neigh;
        if (down_exists && j < 4            && ((down >> (j+1)) & 1) == midbit) ++neigh;

        if (neigh > 4) ++bits;
    }
    return bits;
}

/* precompute transition masks once */
static void precompute(void)
{
    static bool done = false;
    if (done) return;
    done = true;

    for (int prev = 0; prev <= NO_ROW; ++prev) {
        for (int cur = 0; cur < ROWS; ++cur) {
            uint32_t z = 0, o = 0;
            for (int next = 0; next < ROWS; ++next) {
                uint8_t b = compute_bits(prev, (uint8_t)cur, next);
                if (b == 0)      z |= (1U << next);
                else if (b == 1) o |= (1U << next);
            }
            zero_mask[prev][cur] = z;
            one_mask [prev][cur] = o;
            final_bits[prev][cur] = compute_bits(prev, (uint8_t)cur, NO_ROW);
        }
    }
}

/* the required sequence function */
seq_t a(idx_t n)
{
    precompute();

    static seq_t cur[33][32][2];
    static seq_t nxt[33][32][2];

    /* clear DP tables */
    for (int i = 0; i <= NO_ROW; ++i)
        for (int j = 0; j < ROWS; ++j)
            for (int k = 0; k < 2; ++k)
                cur[i][j][k] = 0;

    /* initial states: first row selected, no exception yet */
    for (int r0 = 0; r0 < ROWS; ++r0)
        cur[NO_ROW][r0][0] = 1;

    /* build DP */
    for (idx_t step = 0; step + 1 < n; ++step) {
        /* clear next table */
        for (int i = 0; i <= NO_ROW; ++i)
            for (int j = 0; j < ROWS; ++j)
                for (int k = 0; k < 2; ++k)
                    nxt[i][j][k] = 0;

        for (int prev = 0; prev <= NO_ROW; ++prev) {
            for (int cur_row = 0; cur_row < ROWS; ++cur_row) {
                seq_t c0 = cur[prev][cur_row][0];
                seq_t c1 = cur[prev][cur_row][1];
                if (!c0 && !c1) continue;

                if (c0) {
                    uint32_t zmask = zero_mask[prev][cur_row];
                    while (zmask) {
                        int next = __builtin_ctz(zmask);
                        zmask &= zmask - 1;
                        nxt[cur_row][next][0] += c0;
                    }
                    uint32_t omask = one_mask[prev][cur_row];
                    while (omask) {
                        int next = __builtin_ctz(omask);
                        omask &= omask - 1;
                        nxt[cur_row][next][1] += c0;
                    }
                }

                if (c1) {
                    uint32_t zmask = zero_mask[prev][cur_row];
                    while (zmask) {
                        int next = __builtin_ctz(zmask);
                        zmask &= zmask - 1;
                        nxt[cur_row][next][1] += c1;
                    }
                }
            }
        }

        /* copy nxt to cur */
        for (int i = 0; i <= NO_ROW; ++i)
            for (int j = 0; j < ROWS; ++j)
                for (int k = 0; k < 2; ++k)
                    cur[i][j][k] = nxt[i][j][k];
    }

    /* accumulate final answer */
    seq_t result = 0;
    for (int prev = 0; prev <= NO_ROW; ++prev) {
        for (int cur_row = 0; cur_row < ROWS; ++cur_row) {
            seq_t c0 = cur[prev][cur_row][0];
            seq_t c1 = cur[prev][cur_row][1];
            if (c0 && final_bits[prev][cur_row] == 1)
                result += c0;
            if (c1 && final_bits[prev][cur_row] == 0)
                result += c1;
        }
    }

    /* correct double counting */
    result >>= 1;
    return result;
}
