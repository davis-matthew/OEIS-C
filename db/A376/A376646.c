/*
 * A376646  Number of solutions to x + y == x^2 + y^2 (mod n) with x <= y.
 * https://oeis.org/A376646
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, look
 * Formula:   a(n) counts solutions to x + y == x^2 + y^2 (mod n) with 0 ≤ x,y
 *            < n and x ≤ y. We compute f(t) = t(t-1) mod n. Count occurrences
 *            of each residue value. Then answer = ∑_{v} (cnt[v] * cnt[n-v]) /
 *            2, with special handling for v == n-v to include pairs where x =
 *            y
 * Range:     u8 n <= 167, u16 n <= 4000, u32 n <= 4000, u64 n <= 4000,
 *            u128 n <= 4000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n) {
    /* Allocate array for counts of f(x) residues. */
    uint64_t *cnt = (uint64_t *)malloc(n * sizeof(uint64_t));
    if (!cnt) return 0;  /* allocation failure: return 0 (unlikely in tests) */
    for (idx_t i = 0; i < n; ++i) cnt[i] = 0;

    /* Compute f(x) = x*(x-1) mod n and count. */
    for (idx_t x = 0; x < n; ++x) {
        uint64_t prod = (uint64_t)x * (uint64_t)(x - 1);
        uint64_t v = prod % (uint64_t)n;
        cnt[v] += 1;
    }

    /* Count unordered solutions with x <= y. */
    seq_t total = 0;
    for (idx_t v = 0; v < n; ++v) {
        uint64_t cv = cnt[v];
        if (cv == 0) continue;
        idx_t w = (n - v) % n;   /* w = -v mod n */
        if (v < w) {
            seq_t add = (seq_t)cv * (seq_t)cnt[w];
            total += add;
        } else if (v == w) {
            seq_t cv_t = (seq_t)cv;
            /* Count pairs from same residue: choose 2 + diagonal (x=y) */
            seq_t add = cv_t * (cv_t + 1) / 2;
            total += add;
        }
        /* for v > w, already counted when iterating smaller residue */
    }

    free(cnt);
    return total;
}
