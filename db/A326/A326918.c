/*
 * A326918  Squares visited by a knight moving on a single-digit square-spiral
 *          numbered board where the knight moves to the smallest numbered
 *          unvisited square; the minimum distance from the origin is used if
 *          the square numbers are equal; the smallest spiral number ordering
 *          is used if the distances are equal.
 * https://oeis.org/A326918
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, fini, full
 * Formula:   a(n) = A007376(A326916(n))
 * Range:     u8 n <= 1069, u16 n <= 1069, u32 n <= 1069, u64 n <= 1069,
 *            u128 n <= 1069, big any n
 * Helpers:   digit_of, rank_of
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence computed by simulation of knight moves on a single‑digit spiral board.
   The board digit at rank r is the r-th digit of the infinite concatenation of
   the decimal representations of the non‑negative integers 0,1,2,… (A007376). */
#include <stdlib.h>
#include "../utils/digit_of.h"
#include "../utils/rank_of.h"

static const int DX[8] = { 1,  2,  2,  1, -1, -2, -2, -1 };
static const int DY[8] = { 2,  1, -1, -2, -2, -1,  1,  2 };

seq_t a(idx_t n) {
    /* maximum number of squares that may be visited (sequence ends at 1069 steps) */
    enum { MAX_VIS = 2000 };
    static seq_t digits[MAX_VIS];
    static int xs[MAX_VIS], ys[MAX_VIS];
    static idx_t seq_len = 0;
    static int initialized = 0;

    if (!initialized) {
        /* one‑time simulation of the knight walk */
        seq_len = 1;
        xs[0] = 0; ys[0] = 0;
        digits[0] = digit_of(0);

        /* simulate up to the point where no unvisited knight move exists */
        for (idx_t step = 1; step < MAX_VIS; ++step) {
            int cx = xs[step - 1];
            int cy = ys[step - 1];

            int bestx = 0, besty = 0;
            seq_t bestDigit = 10;           /* larger than any possible digit (0–9) */
            idx_t bestDist = (idx_t)-1;     /* maximal value */
            idx_t bestRank = 0;

            for (int m = 0; m < 8; ++m) {
                int nx = cx + DX[m];
                int ny = cy + DY[m];

                /* check if (nx,ny) is already visited */
                int visited = 0;
                for (idx_t i = 0; i < seq_len; ++i)
                    if (xs[i] == nx && ys[i] == ny) { visited = 1; break; }
                if (visited) continue;

                idx_t rank = rank_of(nx, ny);
                seq_t digit = digit_of(rank);
                idx_t dist = (idx_t)nx * nx + (idx_t)ny * ny;

                if (digit < bestDigit) {
                    bestDigit = digit;
                    bestDist  = dist;
                    bestRank  = rank;
                    bestx     = nx;
                    besty     = ny;
                } else if (digit == bestDigit) {
                    if (dist < bestDist) {
                        bestDist = dist;
                        bestRank = rank;
                        bestx    = nx;
                        besty    = ny;
                    } else if (dist == bestDist && rank < bestRank) {
                        bestRank = rank;
                        bestx    = nx;
                        besty    = ny;
                    }
                }
            }

            if (bestDigit == 10) break;   /* no unvisited neighbour */
            xs[seq_len] = bestx;
            ys[seq_len] = besty;
            digits[seq_len] = bestDigit;
            ++seq_len;
        }
        initialized = 1;
    }

    if (n < seq_len) return digits[n];
    /* for n beyond the end of the finite sequence we return 0 */
    return (seq_t)0;
}
