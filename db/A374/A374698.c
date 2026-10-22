/*
 * A374698  Numbers k such that the leaders of strictly increasing runs in the
 *          k-th composition in standard order are distinct.
 * https://oeis.org/A374698
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 62, u16 n <= 62, u32 n <= 62, u64 n <= 62, u128 n <= 62,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computes OEIS A374698: numbers k such that the leaders of strictly
   increasing runs in the k-th composition in standard order are distinct.
   The algorithm enumerates k ascending, checks the property via the
   binary‑positions representation of k and returns the n‑th such k. */
#include <stdbool.h>

static bool is_valid(idx_t k)
{
    /* Positions of set bits in k (1‑based). */
    idx_t pos[64];
    int m = 0;
    for (int i = 0; i < 64; ++i) {
        if (k & ((idx_t)1 << i))
            pos[m++] = (idx_t)(i + 1);
    }
    if (m == 0) /* empty composition is always valid */
        return true;

    /* Gaps between consecutive positions (differences). */
    idx_t g[64];
    g[0] = pos[0];
    for (int j = 1; j < m; ++j)
        g[j] = pos[j] - pos[j - 1];

    /* Detect leaders in the reversed composition. */
    bool seen[65] = {false};   /* parts are <= 64 */
    idx_t prev = (idx_t)-1;    /* no previous part yet */

    for (int idx = m - 1; idx >= 0; --idx) {
        idx_t part = g[idx];
        if (prev == (idx_t)-1) {
            /* First part of the composition: new run. */
            if (seen[(int)part])
                return false;
            seen[(int)part] = true;
        } else {
            if (part > prev) {
                /* Same run continues, nothing to check. */
            } else {
                /* New run starts: part is a new leader. */
                if (seen[(int)part])
                    return false;
                seen[(int)part] = true;
            }
        }
        prev = part;
    }
    return true;
}

seq_t a(idx_t n)
{
    /* n >= 1 by definition of the sequence. */
    idx_t count = 0;
    idx_t k = 0;
    while (true) {
        if (is_valid(k)) {
            ++count;
            if (count == n)
                return (seq_t)k;
        }
        ++k;
    }
}
