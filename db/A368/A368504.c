/*
 * A368504  Square array T(n,k), n >= 0, k >= 0, read by antidiagonals
 *          downwards, where T(n,k) = Sum_{j=0..n} k^(n-j) * j^k.
 * https://oeis.org/A368504
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   T(0,k) = 0^k (with 0^0 = 1) T(n,k) = k*T(n-1,k) + n^k a(n) is
 *            the element T(i,j) where i+j equals the antidiagonal index of n,
 *            read by anti‑diagonals downward
 * Range:     u8 n <= 38, u16 n <= 65, u32 n <= 65, u64 n <= 65, u128 n <= 65,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    /* Find the diagonal s such that n lies in
       s*(s+1)/2 ≤ n < (s+1)*(s+2)/2. */
    idx_t s = 0;
    while (((s + 1) * (s + 2)) / 2 <= n)
        ++s;

    idx_t stride = s + 1;                /* width of the square */
    size_t sz = (size_t)stride;

    /* Allocate two rows for dynamic programming. */
    seq_t *prev = (seq_t *)malloc(sz * sizeof(seq_t));
    seq_t *curr = (seq_t *)malloc(sz * sizeof(seq_t));
    if (!prev || !curr) { free(prev); free(curr); return 0; }

    /* Initialise row 0: T(0,k) = 0^k. */
    for (idx_t k = 0; k < stride; ++k)
        prev[k] = (k == 0) ? (seq_t)1 : (seq_t)0;

    /* Compute diagonal indices. */
    idx_t start  = (s * (s + 1)) / 2;     /* first index on diagonal s */
    idx_t offset = n - start;             /* offset within that diagonal */
    /* i = offset, j = s - offset. */

    if (offset == 0) {                    /* i = 0 */
        seq_t res = prev[s];
        free(prev);
        free(curr);
        return res;
    }

    seq_t result = 0;
    /* Build rows 1 .. s. */
    for (idx_t i = 1; i <= s; ++i) {
        seq_t power = (seq_t)1;           /* i^0 */
        for (idx_t j = 0; j < stride; ++j) {
            if (j == 0)
                curr[0] = (seq_t)1;      /* T(i,0) = 1 */
            else {
                power = power * (seq_t)i;/* i^j */
                curr[j] = (seq_t)j * prev[j] + power;
            }
        }
        if (i == offset) {
            idx_t j_needed = s - offset;  /* j = s - i */
            result = curr[j_needed];
            break;                         /* found the required term */
        }
        /* Prepare for next row. */
        seq_t *tmp = prev;
        prev = curr;
        curr = tmp;
    }

    free(prev);
    free(curr);
    return result;
}
