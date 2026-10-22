/*
 * A350438  a(n) is the number of integers that can be represented in a
 *          7-segment display by using only n segments (version A010371).
 * https://oeis.org/A350438
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, base, easy
 * Formula:   a(n) = pos[n] + pos[n-1] for all n ≠ 7 a(7) = pos[7] + pos[6] -
 *            1 where pos[s] counts the number of non‑negative integers that
 *            can be displayed on a 7‑segment display with exactly s segments
 *            (the digit 7 uses 4 segments). The digits have costs: 0→6, 1→2,
 *            2→5, 3→5, 4→4, 5→5, 6→6, 7→4, 8→7, 9→6. po...
 * Range:     u8 n <= 12, u16 n <= 23, u32 n <= 40, u64 n <= 40, u128 n <= 40,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n) {
    /* Segment costs for digits 0–9 (7 uses 4 segments) */
    static const int cost[10] = {6,2,5,5,4,5,6,4,7,6};

    /* Memoised arrays of sequence values */
    static seq_t *g = NULL;               /* g[s] = number of strings of digits (0‑9) with total cost s (order matters) */
    static seq_t *pos = NULL;             /* pos[s] = number of non‑negative integers with total cost s */
    static size_t max_computed = 0;       /* largest s for which g[pos] have been filled */

    if (n == 0) return 0;                /* a(0) = 0  */

    size_t needed = (size_t)n;           /* we only need values up to n (pos[n] and pos[n‑1]) */

    /* Allocate or grow the arrays if necessary */
    if (g == NULL) {
        g = (seq_t *)calloc(needed + 1, sizeof(seq_t));
        pos = (seq_t *)calloc(needed + 1, sizeof(seq_t));
        if (!g || !pos) return 0;       /* out of memory – unlikely in the test suite */
        g[0] = 1;                        /* one empty string */
        pos[0] = 0;                      /* no integer uses 0 segments */
    } else if (max_computed < needed) {
        g = (seq_t *)realloc(g, (needed + 1) * sizeof(seq_t));
        pos = (seq_t *)realloc(pos, (needed + 1) * sizeof(seq_t));
        if (!g || !pos) return 0;
        for (size_t i = max_computed + 1; i <= needed; ++i) {
            g[i] = 0;
            pos[i] = 0;
        }
    }

    /* Compute values up to `needed` */
    for (size_t s = max_computed + 1; s <= needed; ++s) {
        /* g[s] = Σ g[s – cost[d]] over all digits d (0‑9) with cost[d] ≤ s */
        seq_t g_total = 0;
        for (int d = 0; d <= 9; ++d) {
            int c = cost[d];
            if (s >= (size_t)c) g_total += g[s - c];
        }
        g[s] = g_total;

        /* pos[s] = (s == cost[0] ? 1 : 0) + Σ g[s – cost[f]] for f = 1..9 */
        seq_t pos_total = (s == (size_t)cost[0]) ? 1 : 0;
        for (int f = 1; f <= 9; ++f) {
            int c = cost[f];
            if (s >= (size_t)c) pos_total += g[s - c];
        }
        pos[s] = pos_total;
    }
    max_computed = needed;

    /* Final value of the sequence */
    seq_t result = pos[n];
    result += pos[n - 1];
    if (n == 7) result -= 1; /* exclude the representation of –0 */

    return result;
}
