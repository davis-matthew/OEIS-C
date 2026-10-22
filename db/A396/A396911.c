/*
 * A396911  Shortest path indices during walk over golden ratio phyllotaxis
 *          starting at the center.
 * https://oeis.org/A396911
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, nice
 * Formula:   a(n) computed by simulating the greedy walk in Vogel's *
 *            phyllotaxis lattice (radial exponent 1/2, golden angle). a(0) =
 *            0
 * Range:     u8 n <= 13, u16 n <= 216, u32 n <= 999, u64 n <= 999,
 *            u128 n <= 999, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include <stdint.h>
#include <stdlib.h>
#include <math.h>

/* enlarge the visited‑bit array so that it can contain idx */
static inline void ensure_capacity(uint64_t **bits, size_t *words, idx_t idx)
{
    size_t w = idx >> 6;
    if (w < *words) return;
    size_t new_words = (*words) ? (*words << 1) : 1;
    while (w >= new_words) new_words <<= 1;
    uint64_t *nb = (uint64_t*)realloc(*bits, new_words * sizeof(uint64_t));
    if (!nb) abort();
    for (size_t i = *words; i < new_words; ++i) nb[i] = 0;
    *bits = nb;
    *words = new_words;
}

/* test whether idx has been visited */
static inline bool is_visited(const uint64_t *bits, size_t words, idx_t idx)
{
    size_t w = idx >> 6;
    return (w < words) && ((bits[w] >> (idx & 63)) & 1ULL);
}

/* mark idx as visited */
static inline void set_visited(uint64_t *bits, size_t words, idx_t idx)
{
    size_t w = idx >> 6;
    bits[w] |= 1ULL << (idx & 63);
}

seq_t a(idx_t n)
{
    if (n == 0) return 0;

    /* constants for the golden angle Vogel lattice */
    const double PI  = acos(-1.0);
    const double PHI = (1.0 + sqrt(5.0)) / 2.0;
    const double THETA = 2.0 * PI * (1.0 - 1.0 / PHI);   /* golden angle in rad */

    /* upper bound for any index that may be visited in the first n moves */
    idx_t lim = (idx_t)(((__uint128_t)n * n * 3) / 2 + 10);

    size_t words = (lim >> 6) + 1;
    uint64_t *visited = (uint64_t*)calloc(words, sizeof(uint64_t));
    if (!visited) abort();

    idx_t cur = 0;
    set_visited(visited, words, cur);

    for (idx_t step = 1; step <= n; ++step)
    {
        double r_cur = sqrt((double)cur);
        double x_cur = r_cur * cos((double)cur * THETA);
        double y_cur = r_cur * sin((double)cur * THETA);

        double bestDist2 = 1e300;
        idx_t bestIdx = 0;      /* will be overwritten */

        /* search indices below the current one */
        for (idx_t offset = 1; offset <= cur; ++offset)
        {
            idx_t idx = cur - offset;
            if (!is_visited(visited, words, idx))
            {
                double r = sqrt((double)idx);
                double dx = r * cos((double)idx * THETA) - x_cur;
                double dy = r * sin((double)idx * THETA) - y_cur;
                double d2 = dx * dx + dy * dy;
                if (d2 < bestDist2 || (d2 == bestDist2 && idx < bestIdx))
                {
                    bestDist2 = d2;
                    bestIdx   = idx;
                }
            }
            double r = sqrt((double)idx);
            double rad = r_cur - r;
            if (rad * rad > bestDist2) break;
        }

        /* search indices above the current one */
        for (idx_t offset = 1;; ++offset)
        {
            idx_t idx = cur + offset;
            if (!is_visited(visited, words, idx))
            {
                double r = sqrt((double)idx);
                double dx = r * cos((double)idx * THETA) - x_cur;
                double dy = r * sin((double)idx * THETA) - y_cur;
                double d2 = dx * dx + dy * dy;
                if (d2 < bestDist2 || (d2 == bestDist2 && idx < bestIdx))
                {
                    bestDist2 = d2;
                    bestIdx   = idx;
                }
            }
            double r = sqrt((double)idx);
            double rad = r - r_cur;
            if (rad * rad > bestDist2) break;
        }

        cur = bestIdx;
        ensure_capacity(&visited, &words, cur);
        set_visited(visited, words, cur);
    }

    free(visited);
    return cur;
}
