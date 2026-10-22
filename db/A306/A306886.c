/*
 * A306886  Number of distinct nonzero values taken by (x1-x2)^2 + (y1-y2)^2
 *          for x1, y1, x2, y2 integers with 0 <= (x1,y1) <= n and -n <=
 *          (x2,y2) <= 0.
 * https://oeis.org/A306886
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) is the number of distinct non‑zero values taken by dx²+dy²
 *            where 0≤dx,dy≤2n and dx,dy are integers
 * Range:     u8 n <= 11, u16 n <= 48, u32 n <= 48, u64 n <= 48, u128 n <= 48,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>

seq_t a(idx_t n)
{
    if (n == 0)
        return 0;  /* a(0) = 0 */

    /* maximum coordinate difference */
    const idx_t maxA = 2 * n;              /* 0 … 2n */
    /* maximum possible sum of squares */
    const idx_t maxSum = (idx_t)8 * n * n; /* (2n)² + (2n)² = 8n² */

    /* number of 64‑bit words needed for bitset */
    const size_t words = ((size_t)maxSum >> 6) + 1;

    /* bitset: one bit per possible sum from 0 … maxSum */
    uint64_t *bits = (uint64_t *)calloc(words, sizeof(uint64_t));
    if (!bits)
        return 0;   /* allocation failed */

    /* pre‑compute squares of 0 … maxA */
    idx_t *sq = (idx_t *)calloc((size_t)maxA + 1, sizeof(idx_t));
    if (!sq) {
        free(bits);
        return 0;
    }
    for (idx_t i = 0; i <= maxA; ++i)
        sq[i] = i * i;

    /* enumerate all pairs (dx,dy) and set the corresponding bit */
    for (idx_t dx = 0; dx <= maxA; ++dx) {
        idx_t dx_sq = sq[dx];
        for (idx_t dy = 0; dy <= maxA; ++dy) {
            if (dx == 0 && dy == 0)          /* skip zero distance */
                continue;
            idx_t sum = dx_sq + sq[dy];
            size_t word = (size_t) (sum >> 6);
            uint64_t mask = 1ULL << (sum & 63ULL);
            bits[word] |= mask;
        }
    }

    /* count set bits (distinct sums), then free resources */
    idx_t cnt = 0;
    for (size_t w = 0; w < words; ++w)
        cnt += (idx_t) __builtin_popcountll(bits[w]);

    free(bits);
    free(sq);

    return (seq_t) cnt;
}
