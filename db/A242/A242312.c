/*
 * A242312  Digital roots in Pascal's triangle, triangle read by rows, 0 <= k
 *          <= n.
 * https://oeis.org/A242312
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, tabl, base
 * Formula:   a(n) = digital_root(binomial(i,j)) where (i,j) satisfies n =
 *            i*(i+1)/2 + j, 0 <= j <= i. digital_root(m) = (m % 9 == 0) ? 9 :
 *            (m % 9)
 * Range:     u8 n <= 8000, u16 n <= 8000, u32 n <= 8000, u64 n <= 8000,
 *            u128 n <= 8000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>

seq_t a(idx_t n)
{
    /* Find row i and column j corresponding to index n. */
    idx_t i = 0;
    idx_t cumulative = 0; /* number of entries before row i */
    while (cumulative + i + 1 <= n) {
        cumulative += i + 1;
        ++i;
    }
    idx_t j = n - cumulative; /* 0 <= j <= i */

    /* Compute binomial(i,j) modulo 9 using Pascal's triangle recurrence. */
    if (i == 0) {
        /* Row 0 contains only 1 */
        seq_t val = 1;
        return val;
    }

    /* Allocate two rows, each of length i+1. We only need uint8_t values. */
    uint8_t *prev = (uint8_t *)malloc((i + 1) * sizeof(uint8_t));
    uint8_t *curr = (uint8_t *)malloc((i + 1) * sizeof(uint8_t));
    if (!prev || !curr) {
        /* Out of memory; return 0 as a safe fallback. */
        if (prev) free(prev);
        if (curr) free(curr);
        return 0;
    }

    /* Initialize row 0 */
    prev[0] = 1; /* binomial(0,0) == 1 */
    /* Build rows up to i */
    for (idx_t r = 1; r <= i; ++r) {
        curr[0] = 1; /* binomial(r,0) == 1 */
        curr[r] = 1; /* binomial(r,r) == 1 */
        for (idx_t k = 1; k < r; ++k) {
            curr[k] = (prev[k - 1] + prev[k]) % 9;
        }
        /* Prepare for next iteration */
        uint8_t *tmp = prev;
        prev = curr;
        curr = tmp;
    }

    /* At this point, prev contains row i modulo 9. */
    uint8_t mod9 = prev[j] % 9;
    seq_t result = (mod9 == 0) ? 9 : mod9;

    free(prev);
    free(curr);
    return result;
}
