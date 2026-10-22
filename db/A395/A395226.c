/*
 * A395226  a(n) = number of 4-tuples (w, x, y, z) such that w*x + y*z = n,
 *          where w, x, y, z are positive Fibonacci numbers satisfying w <= x
 *          and y <= z.
 * https://oeis.org/A395226
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 86, u16 n <= 9065, u32 n <= 9065, u64 n <= 9065,
 *            u128 n <= 9065, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* 
 * Formula: a(n) = number of 4‑tuples (w, x, y, z) of positive Fibonacci
 * numbers with w ≤ x, y ≤ z, such that w*x + y*z = n.
 * Implementation: enumerate all unordered Fibonacci pairs for the two
 * products and count matches.
 */

#include <stdlib.h>

/* Computes a(n) */
seq_t a(idx_t n)
{
    /* Generate unique Fibonacci numbers ≤ n */
    idx_t fibCnt = 0;
    seq_t fibVals[32];  /* enough for n up to about 10^6 */
    seq_t a1 = 1, b1 = 1;
    seq_t last = 0;
    while (a1 <= n) {
        if (a1 != last) {
            fibVals[fibCnt++] = a1;
            last = a1;
        }
        seq_t tmp = a1 + b1;
        a1 = b1;
        b1 = tmp;
    }

    /* cnt[p] will hold the number of unordered Fibonacci pairs (y, z)
     * with product y*z = p (0 ≤ p ≤ n). */
    size_t len = (size_t)(n + 1);
    seq_t *cnt = (seq_t *)calloc(len, sizeof(seq_t));
    if (!cnt) return 0;            /* allocation failure – return 0 */

    for (idx_t i = 0; i < fibCnt; ++i) {
        for (idx_t j = i; j < fibCnt; ++j) {
            seq_t prod = fibVals[i] * fibVals[j];
            if (prod > (seq_t)n) break;
            cnt[(size_t)prod] += 1;
        }
    }

    /* Count tuples: for each unordered pair (w, x) find how many
     * unordered pairs (y, z) satisfy w*x + y*z = n. */
    seq_t ans = 0;
    for (idx_t i = 0; i < fibCnt; ++i) {
        for (idx_t j = i; j < fibCnt; ++j) {
            seq_t prod = fibVals[i] * fibVals[j];
            if (prod > (seq_t)n) break;
            idx_t remaining = (idx_t)(n - prod);  /* y*z must equal this */
            ans += cnt[(size_t)remaining];
        }
    }

    free(cnt);
    return ans;
}
