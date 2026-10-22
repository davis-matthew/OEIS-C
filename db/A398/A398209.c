/*
 * A398209  Triangle read by rows: For n >= 1, T(n,k) is the number of
 *          distinct entries q of the n X n multiplication table such that
 *          floor(q/n) = k, for 0 <= k <= n, with T(0,0) = 0.
 * https://oeis.org/A398209
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   a(n) = T(r,k) where r and k are derived from the triangular
 *            indexing of the rows of the triangle read by rows. The value is
 *            computed by explicitly enumerating the distinct products of the
 *            r×r multiplication table and counting how many fall into each
 *            interval [k*n,(k+1)*n‑1]
 * Range:     u8 n <= 90, u16 n <= 90, u32 n <= 90, u64 n <= 90, u128 n <= 90,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>
#include <math.h>

seq_t a(idx_t n)
{
    if (n == 0) return 0;                     /* T(0,0) = 0 */

    /* Find the row r such that r*(r+1)/2 <= n < (r+1)*(r+2)/2 */
    idx_t r = (idx_t)((sqrt((double)n * 8.0 + 1.0) - 1.0) / 2.0);
    while ((r + 1) * (r + 2) / 2 <= n) ++r;
    while (r * (r + 1) / 2 > n) --r;

    idx_t k = n - r * (r + 1) / 2;            /* column index within the row */

    /* Bitset for marking seen products.
       Number of bits needed is r*r + 1 (to include product r*r).               */
    uint64_t max_product = (uint64_t)r * (uint64_t)r;
    size_t bitset_bytes = (size_t)(max_product + 8) >> 3;
    uint8_t *bitset = (uint8_t *)calloc(bitset_bytes, sizeof(uint8_t));
    if (!bitset) return 0;                    /* allocation failure – safe fallback */

    /* Array for counts of products in each interval [k*n,(k+1)*n-1]. */
    seq_t *cnt = (seq_t *)calloc((size_t)(r + 1), sizeof(seq_t));
    if (!cnt) {
        free(bitset);
        return 0;
    }

    /* Enumerate distinct products i*j with 1 <= i <= j <= r.            */
    for (idx_t i = 1; i <= r; ++i) {
        for (idx_t j = i; j <= r; ++j) {
            uint64_t prod = (uint64_t)i * (uint64_t)j;   /* 0 <= prod <= r*r */
            size_t byte_idx = prod >> 3;
            uint8_t  bit_mask = 1u << (prod & 7u);

            if (!(bitset[byte_idx] & bit_mask)) {       /* new distinct product */
                bitset[byte_idx] |= bit_mask;
                cnt[prod / r]++;                       /* prod / r == floor(prod/r) */
            }
        }
    }

    seq_t result = cnt[k];
    free(bitset);
    free(cnt);
    return result;
}
