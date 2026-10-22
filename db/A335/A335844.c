/*
 * A335844  Squares visited by a knight moving on a square-spiral numbered
 *          board where the knight moves to the unvisited square containing
 *          the spiral number with the fewest divisors. In case of a tie it
 *          chooses the square with the lowest spiral number.
 * https://oeis.org/A335844
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, fini, full, walk
 * Formula:   Computes a(n) for OEIS A335844 by simulating the knight's walk *
 *            on the square‑spiral numbering. The function returns the number
 *            * of the square visited on the n‑th step (n ≥ 1). If n exceeds
 *            the * length of the walk the function returns 0
 * Range:     u8 n <= 38, u16 n <= 529, u32 n <= 529, u64 n <= 529,
 *            u128 n <= 529, big any n
 * Helpers:   coord_to_number
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdint.h>
#include <stdbool.h>
#include "../utils/coord_to_number.h"

#define MAX_NUM 20000          /*  safe upper bound for visited squares   */
/* Move offsets for a knight */
static const int DX[8] = { 1,  2, -1, -2,  1,  2, -1, -2 };
static const int DY[8] = { 2,  1,  2,  1, -2, -1, -2, -1 };

/* Sequence storage (A335844 has ≤528 terms) */
static seq_t seq_values[600];
static size_t seq_len = 0;

/* Visited square flags */
static bool visited[MAX_NUM + 1] = { false };

/* --------------------------------------------------------------------- */
/* Utility: divisor count for uint64_t numbers */
static uint64_t divisor_count_uint64(uint64_t n)
{
    uint64_t cnt = 1;
    uint64_t p, m;
    for (p = 2; p * p <= n; ++p) {
        if (n % p == 0) {
            uint64_t e = 0;
            while (n % p == 0) {
                n /= p;
                ++e;
            }
            cnt *= (e + 1);
        }
    }
    if (n > 1) cnt *= 2;  /* last prime factor */
    return cnt;
}

/* --------------------------------------------------------------------- */

/* --------------------------------------------------------------------- */
/* Build the entire walk once (memoized)                                */
static void build_sequence(void)
{
    int cx = 0, cy = 0;          /* current knight position (x,y)      */
    visited[1] = true;
    seq_values[0] = (seq_t)1;
    seq_len = 1;

    while (true) {
        seq_t best_num   = 0;
        uint64_t best_divcnt = UINT64_MAX;
        int best_x = 0, best_y = 0;

        for (int d = 0; d < 8; ++d) {
            int nx = cx + DX[d];
            int ny = cy + DY[d];
            seq_t cand = coord_to_number(nx, ny);
            if (cand < 1) continue;

            size_t idx = (size_t)cand;
            if (idx <= (size_t)MAX_NUM && visited[idx]) continue;

            uint64_t dcnt = divisor_count_uint64((uint64_t)cand);
            if (dcnt < best_divcnt || (dcnt == best_divcnt && cand < best_num)) {
                best_divcnt = dcnt;
                best_num = cand;
                best_x = nx;
                best_y = ny;
            }
        }

        if (best_num == 0) break;       /* no unvisited neighbour */

        visited[(size_t)best_num] = true;
        seq_values[seq_len++]  = best_num;
        cx = best_x;
        cy = best_y;
    }
}

/* --------------------------------------------------------------------- */
/* Desired function exported for the judge                                */
seq_t a(idx_t n)
{
    static bool built = false;
    if (!built) { build_sequence(); built = true; }

    if (n == 0) return 0;
    size_t idx = (size_t)(n - 1);
    if (idx < seq_len) return seq_values[idx];
    return 0;   /* undefined for n beyond the finite walk */
}
