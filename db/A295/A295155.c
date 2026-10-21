/*
 * A295155  Numbers that have exactly seven representations as a sum of five
 *          nonnegative squares.
 * https://oeis.org/A295155
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, fini, full
 * Formula:   not stated
 * Range:     u8 n <= 9, u16 n <= 9, u32 n <= 9, u64 n <= 9, u128 n <= 9,
 *            big any n
 * Helpers:   seq_alloc
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include "../utils/seq_alloc.h"

/* Sequence: a(n) = the nth number that has exactly seven representations
   as a sum of five non‑negative squares (order of the squares ignored). */
seq_t a(idx_t n) {
    /* static cache of the full sequence */
    static seq_t *seq   = NULL;
    static size_t seq_len = 0;
    static const idx_t NMAX = 5408;   /* largest possible n in the sequence */

    if (seq == NULL) {
        /* 1. determine the largest square needed */
        int maxI = 0;
        while ((long long)(maxI + 1) * (maxI + 1) <= NMAX) ++maxI;

        /* 2. pre‑compute the squares */
        int squares[74];
        for (int i = 0; i <= maxI; ++i)
            squares[i] = i * i;

        /* 3. count unordered 5‑tuples of squares summing to each value <= NMAX */
        int counts[NMAX + 1];
        for (int i = 0; i <= NMAX; ++i)
            counts[i] = 0;

        for (int i = 0; i <= maxI; ++i) {
            int s1 = squares[i];
            if (s1 > NMAX) break;
            for (int j = i; j <= maxI; ++j) {
                int s2 = s1 + squares[j];
                if (s2 > NMAX) break;
                for (int k = j; k <= maxI; ++k) {
                    int s3 = s2 + squares[k];
                    if (s3 > NMAX) break;
                    for (int l = k; l <= maxI; ++l) {
                        int s4 = s3 + squares[l];
                        if (s4 > NMAX) break;
                        for (int m = l; m <= maxI; ++m) {
                            int sum = s4 + squares[m];
                            if (sum > NMAX) break;
                            ++counts[sum];
                        }
                    }
                }
            }
        }

        /* 4. collect all numbers with exactly 7 representations */
        seq_len = 0;
        for (int i = 0; i <= NMAX; ++i)
            if (counts[i] == 7) ++seq_len;

        seq = seq_alloc(seq_len);
        size_t idx = 0;
        for (int i = 0; i <= NMAX; ++i)
            if (counts[i] == 7)
                seq[idx++] = (seq_t)i;
    }

    /* 5. answer the requested index (offset 1) */
    if (n == 0 || n > seq_len)
        return (seq_t)0;          /* out of bounds: return 0 */
    return seq[(size_t)(n - 1)];
}
