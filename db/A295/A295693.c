/*
 * A295693  Numbers that have exactly three representations as a sum of six
 *          positive squares.
 * https://oeis.org/A295693
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, more
 * Formula:   not stated
 * Range:     u8 none, u16 n <= 15, u32 n <= 15, u64 n <= 15, u128 n <= 15,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* OEIS A295693: Numbers having exactly three representations as a sum of six positive squares.
   This implementation enumerates all non‑decreasing 6‑tuples of positive squares up to a fixed
   maximum bound and caches the numbers that occur exactly three times.  The maximum bound
   (2000) is comfortably beyond the largest known term in the sequence. */

#include <stdlib.h>
#include <stdint.h>

/* Computes the n-th term of the sequence (1‑based). */
seq_t a(idx_t n) {
    /* Static cache of the sequence values */
    static seq_t *cache = NULL;
    static size_t cache_size = 0;

    /* Build the cache on first call */
    if (cache == NULL) {
        /* The sequence is finite; a safe upper bound is 2000. */
        const idx_t MAX = 2000;

        /* Number of possible square sizes (1^2 .. floor(sqrt(MAX))^2) */
        idx_t M = 0;
        while ((M + 1) * (M + 1) <= MAX) ++M;

        /* Squares of the allowed integers */
        seq_t *squares = (seq_t *)malloc((size_t)(M + 1) * sizeof(seq_t));
        if (!squares) return 0; /* Allocation failure */
        squares[0] = (seq_t)0;
        for (idx_t i = 1; i <= M; ++i)
            squares[i] = (seq_t)i * (seq_t)i;

        /* Count representations: cnt[s] = number of 6‑tuples (a≤b≤c≤d≤e≤f) with sum s. */
        size_t *cnt = (size_t *)calloc((size_t)MAX + 1, sizeof(size_t));
        if (!cnt) { free(squares); return 0; }

        for (idx_t a = 1; a <= M; ++a) {
            seq_t s1 = squares[a];
            if (s1 > MAX - 5) break;
            for (idx_t b = a; b <= M; ++b) {
                seq_t s2 = s1 + squares[b];
                if (s2 > MAX - 4) break;
                for (idx_t c = b; c <= M; ++c) {
                    seq_t s3 = s2 + squares[c];
                    if (s3 > MAX - 3) break;
                    for (idx_t d = c; d <= M; ++d) {
                        seq_t s4 = s3 + squares[d];
                        if (s4 > MAX - 2) break;
                        for (idx_t e = d; e <= M; ++e) {
                            seq_t s5 = s4 + squares[e];
                            if (s5 > MAX - 1) break;
                            for (idx_t f = e; f <= M; ++f) {
                                seq_t sum = s5 + squares[f];
                                if (sum > MAX) break;
                                ++cnt[(size_t)sum];
                            }
                        }
                    }
                }
            }
        }
        free(squares);

        /* Count total number of terms with exactly three representations. */
        size_t tlen = 0;
        for (idx_t v = 6; v <= MAX; ++v)
            if (cnt[v] == 3) ++tlen;

        /* Allocate array for the terms. */
        cache = (seq_t *)malloc(tlen * sizeof(seq_t));
        if (!cache) { free(cnt); return 0; }

        /* Fill the array in increasing order of the value. */
        size_t idx = 0;
        for (idx_t v = 6; v <= MAX; ++v)
            if (cnt[v] == 3) {
                cache[idx++] = (seq_t)v;
            }

        cache_size = tlen;
        free(cnt);
    }

    if (n == 0) return 0;
    if ((size_t)n <= cache_size)
        return cache[(size_t)(n - 1)];
    return 0; /* n beyond the finite length of the sequence */
}
