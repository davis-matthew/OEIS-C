/*
 * A382731  Total number of entries in rows 0,1,...,n of Pascal's triangle not
 *          divisible by 8.
 * https://oeis.org/A382731
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = total number of binomial entries in rows 0..n of Pascal's
 *            * triangle that are NOT divisible by 8. * The function computes
 *            this by generating each row modulo 8 and counting * the non‑zero
 *            entries. The algorithm runs in O(n²) time but is fast * enough
 *            for n ≤ 10 000 (≈50 million additions)....
 * Range:     u8 n <= 23, u16 n <= 65, u32 n <= 65, u64 n <= 65, u128 n <= 65,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>

seq_t a(idx_t n)
{
    if (n == 0)                 /* row 0 has only one entry, C(0,0)=1 */
        return (seq_t)1;

    /* The largest row needed has n+1 elements.  Use a regular byte array
     * because we only need values modulo 8.  The two arrays allow us to
     * build each new row from the previous one.                                   */
    const size_t sz = (size_t)n + 1;
    uint8_t *prev = (uint8_t *)malloc(sz * sizeof(uint8_t));
    uint8_t *curr = (uint8_t *)malloc(sz * sizeof(uint8_t));
    if (!prev || !curr) {        /* allocation failed – fallback */
        free(prev);
        free(curr);
        return (seq_t)0;
    }

    /* Row 0 */
    prev[0] = 1;
    seq_t cumulative = 1;        /* only C(0,0)=1 is non‑divisible by 8 */

    for (size_t r = 1; r <= (size_t)n; ++r) {
        /* First and last elements of row r are 1 */
        curr[0]      = 1;
        curr[r]      = 1;

        /* Interior elements: C(r,k) = C(r-1,k-1)+C(r-1,k) (mod 8) */
        for (size_t k = 1; k < r; ++k) {
            uint8_t sum = prev[k - 1] + prev[k];
            curr[k] = sum & 7;          /* modulo 8 */
        }

        /* Count non‑zero entries in the current row */
        seq_t row_cnt = 0;
        for (size_t k = 0; k <= r; ++k)
            if (curr[k] != 0)
                ++row_cnt;

        cumulative += row_cnt;

        /* Swap row buffers for the next iteration */
        uint8_t *tmp = prev;
        prev = curr;
        curr = tmp;
    }

    /* Clean up allocated memory */
    free(prev);
    free(curr);

    return cumulative;
}
