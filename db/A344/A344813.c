/*
 * A344813  Square of the distance from the origin for a square lattice Moore
 *          neighborhood self-avoiding walk using the rules given in the
 *          Comments.
 * https://oeis.org/A344813
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, fini, walk
 * Formula:   not stated
 * Range:     u8 n <= 70, u16 n <= 70, u32 n <= 70, u64 n <= 70, u128 n <= 70,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computes OEIS A344813 by simulating the self‑avoiding walk on a square lattice.
 * The walk starts at the origin (square #1 at (0,0)), steps north to square #2 at (0,1),
 * and from then on each step chooses among the unvisited Moore–neighbourhood squares.
 * Let S be the sum of the indices of the last two visited squares.
 *   • If S is prime, go to the unvisited neighbour whose squared distance
 *     from the origin is maximum (ties → larger x‑coordinate).
 *   • If S is composite, go to the unvisited neighbour whose squared distance
 *     from the origin is minimum (ties → smaller x‑coordinate).
 * The walk stops once no neighbour remains unvisited (129 squares in total).
 */

#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>

#define MAX_TERMS   129                /* 1 + 128 steps */

static seq_t  s_values[MAX_TERMS + 5];
static int    s_x      [MAX_TERMS + 5];
static int    s_y      [MAX_TERMS + 5];
static size_t s_len     = 0;

/* Small primality test for values ≤ 255 – sufficient for the walk (sum ≤ 255). */
static bool is_prime_small(uint32_t n)
{
    if (n < 2) return false;
    if (n % 2 == 0) return n == 2;
    for (uint32_t d = 3; d * d <= n; d += 2)
        if (n % d == 0) return false;
    return true;
}

/* Ensure the sequence is computed up to index n (or until the walk ends). */
static void ensure_upto(idx_t n)
{
    if (s_len == 0) {                    /* initialise the first two squares */
        s_len = 2;
        s_values[0] = (seq_t)0;          /* a(1) */
        s_values[1] = (seq_t)1;          /* a(2) */
        s_x[0] = 0;  s_y[0] = 0;         /* (0,0)  */
        s_x[1] = 0;  s_y[1] = 1;         /* (0,1)  */
    }

    /* Pre‑compute offsets for the 8 Moore neighbours. */
    static const int dx[8] = { 1,  0, -1, -1, -1,  0,  1,  1 };
    static const int dy[8] = { 1,  1,  1,  0, -1, -1, -1,  0 };

    while (s_len < n && s_len < MAX_TERMS) {
        /* Sum of the last two visited indices (1‑based). */
        uint32_t sum = (uint32_t)(s_len - 1) + (uint32_t)s_len; /* 1‑based indices */
        bool pick_far = is_prime_small(sum);

        int cur_x = s_x[s_len - 1];
        int cur_y = s_y[s_len - 1];

        seq_t best_dist    = pick_far ? (seq_t)0 : (seq_t)(-1);
        int   best_nx = 0, best_ny = 0;
        bool  found = false;

        for (int dir = 0; dir < 8; ++dir) {
            int nx = cur_x + dx[dir];
            int ny = cur_y + dy[dir];

            /* Skip already visited squares. */
            bool visited = false;
            for (size_t j = 0; j < s_len; ++j)
                if (s_x[j] == nx && s_y[j] == ny) { visited = true; break; }
            if (visited) continue;

            /* Compute squared distance using a larger integer type to avoid overflow. */
            uint64_t nx_sq = (uint64_t)nx * nx;
            uint64_t ny_sq = (uint64_t)ny * ny;
            seq_t    dist  = (seq_t)(nx_sq + ny_sq);

            if (!found) {
                best_dist = dist; best_nx = nx; best_ny = ny; found = true;
                continue;
            }

            if (pick_far) {          /* maximize distance, tie → larger x */
                if (dist > best_dist || (dist == best_dist && nx > best_nx)) {
                    best_dist = dist; best_nx = nx; best_ny = ny;
                }
            } else {                 /* minimize distance, tie → smaller x */
                if (dist < best_dist || (dist == best_dist && nx < best_nx)) {
                    best_dist = dist; best_nx = nx; best_ny = ny;
                }
            }
        }

        if (!found) break;          /* walk has terminated */

        s_x[s_len]      = best_nx;
        s_y[s_len]      = best_ny;
        s_values[s_len] = best_dist;
        ++s_len;
    }
}

seq_t a(idx_t n)
{
    if (n < 1) return (seq_t)0;          /* undefined for n<1 */
    ensure_upto(n);
    if ((idx_t)s_len >= n) return s_values[n - 1];
    return (seq_t)0;                     /* beyond the finite walk */
}
