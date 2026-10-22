/*
 * A366418  Number of distinct integers of the form (x^n + y^n) mod n.
 * https://oeis.org/A366418
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 75, u16 n <= 75, u32 n <= 75, u64 n <= 75, u128 n <= 75,
 *            big any n
 * Helpers:   mod_pow
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Compute a(n) = |{ (x^n + y^n) mod n | 0 ≤ x, y < n }| by brute‐force. */
/* The function works for any seq_t (uint8_t … __int128) and for any
   idx_t ≤ 2^64−1. It is not multiplicative; the values are computed
   directly from residues modulo n. */

#include <stdlib.h>
#include <stdbool.h>
#include "../utils/mod_pow.h"

seq_t a(idx_t n)
{
    /* All indices are ≥ 1 by the sequence definition. */
    if (n == 0) return 0;   /* undefined case, but keep safe */

    /* Allocate a table of visited residues for xᶰ mod n. */
    size_t N = (size_t)n;
    bool *seenS = (bool *)calloc(N, sizeof(bool));
    if (!seenS) return 0;   /* allocation failure */

    /* Compute x^n mod n for all x. */
    for (idx_t x = 0; x < n; ++x) {
        seq_t r = mod_pow((seq_t)x, n, (seq_t)n);
        seenS[(size_t)r] = true;
    }

    /* Store distinct residues in a list for later summation. */
    size_t lenS = 0;
    seq_t *listS = (seq_t *)malloc(N * sizeof(seq_t));
    if (!listS) {
        free(seenS);
        return 0;           /* allocation failure */
    }
    for (size_t i = 0; i < N; ++i) {
        if (seenS[i]) {
            listS[lenS++] = (seq_t)i;
        }
    }

    /* Compute all possible sums of two residues modulo n. */
    bool *seenT = (bool *)calloc(N, sizeof(bool));
    if (!seenT) {
        free(seenS);
        free(listS);
        return 0;           /* allocation failure */
    }

    for (size_t i = 0; i < lenS; ++i) {
        seq_t a = listS[i];
        for (size_t j = 0; j < lenS; ++j) {
            seq_t t = a + listS[j];
            if (t >= n) t -= n;   /* modulo n */
            seenT[(size_t)t] = true;
        }
    }

    /* Count the distinct sums. */
    seq_t count = 0;
    for (size_t i = 0; i < N; ++i) {
        if (seenT[i]) ++count;
    }

    /* Clean up and return result. */
    free(seenS);
    free(seenT);
    free(listS);
    return count;
}
