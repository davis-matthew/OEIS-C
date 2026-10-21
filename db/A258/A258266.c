/*
 * A258266  Numbers having only one representation as a sum of the minimal
 *          number of squares, A002828.
 * https://oeis.org/A258266
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   numbers having exactly one representation as a sum of the
 *            minimal number of squares
 * Range:     u8 n <= 67, u16 n <= 67, u32 n <= 67, u64 n <= 67, u128 n <= 67,
 *            big any n
 * Helpers:   int_sqrt
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Compute by dynamic programming: for each value we store the minimal number of squares needed
 * (minC) and whether this minimal number is achieved in a unique way (repC).  Values greater
 * than 1 are encoded as 2 to indicate “two or more” minimal representations.  The sequence
 * is the sorted list of integers with repC = 1.  The routine pre‑computes up to LIMIT
 * (chosen to yield >10000 entries) on the first call. */

#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include "../utils/int_sqrt.h"

seq_t a(idx_t n)
{
    static bool initialized = false;
    static seq_t *seq_terms = NULL;
    static size_t seq_len   = 0;

    if (!initialized)
    {
        /* Upper bound for the DP that covers at least the first 10 000 terms. */
        const idx_t LIMIT = 200000;

        /* minC[v] = minimal number of squares to form v (uint8_t because the value < 255). */
        uint8_t *minC = (uint8_t *)malloc((LIMIT + 1) * sizeof(uint8_t));
        /* repC[v] = 0 = none, 1 = unique minimal, 2 = two or more minimal. */
        uint8_t *repC = (uint8_t *)malloc((LIMIT + 1) * sizeof(uint8_t));
        if (!minC || !repC) {
            /* In case malloc fails, we simply leave the function uninitialized. */
            return (seq_t)0;
        }

        memset(minC, 0xFF, (LIMIT + 1) * sizeof(uint8_t));
        memset(repC, 0,    (LIMIT + 1) * sizeof(uint8_t));
        minC[0] = 0;
        repC[0] = 1;   /* empty sum – one representation */

        idx_t sqrtLim = int_sqrt(LIMIT);
        for (idx_t a = 1; a <= sqrtLim; ++a)
        {
            idx_t sq = a * a;
            for (idx_t cur = sq; cur <= LIMIT; ++cur)
            {
                idx_t prev = cur - sq;
                uint8_t cand = minC[prev] + 1;

                if (cand < minC[cur]) {
                    minC[cur] = cand;
                    repC[cur] = repC[prev];
                } else if (cand == minC[cur]) {
                    /* Found another minimal decomposition.  Mark as >1. */
                    repC[cur] = 2;
                }
            }
        }

        /* Build the sequence of unique‑representation numbers. */
        seq_terms = (seq_t *)malloc((LIMIT + 1) * sizeof(seq_t));
        if (!seq_terms) {
            free(minC);
            free(repC);
            return (seq_t)0;
        }
        seq_len = 0;
        for (idx_t val = 1; val <= LIMIT; ++val)
            if (repC[val] == 1)
                seq_terms[seq_len++] = (seq_t)val;

        /* Free temporary arrays. */
        free(minC);
        free(repC);

        initialized = true;
    }

    if (n == 0 || n > seq_len)
        return (seq_t)0;      /* undefined for indices outside the computed range */
    return seq_terms[(size_t)(n - 1)];
}
