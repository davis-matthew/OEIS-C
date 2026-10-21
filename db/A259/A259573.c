/*
 * A259573  Number of distinct differences in row n of the reciprocity array
 *          of 0.
 * https://oeis.org/A259573
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = |{ ((n-1)+gcd(n,m+1)-gcd(n,m))/2 : 1 ≤ m ≤ n }|
 * Range:     u8 n <= 823, u16 n <= 8588, u32 n <= 8588, u64 n <= 8588,
 *            u128 n <= 8588, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdint.h>
#include <stdbool.h>
#include <stdlib.h>

/* Compute gcd for idx_t (unsigned 64‑bit). */
static inline idx_t _gcd_idx(idx_t a, idx_t b)
{
    while (b != 0) {
        idx_t r = a % b;
        a = b;
        b = r;
    }
    return a;
}

/* Number of distinct differences in row n of the reciprocity array of 0. */
seq_t a(idx_t n)
{
    if (n == 0)    /* safety for unexpected call */
        return 0;

    /* Differences lie in the range [0, n-1], so an array of size n suffices. */
    bool *seen = (bool *)calloc((size_t)n, sizeof(bool));
    if (!seen)     /* allocation failure unlikely for n ≤ 10000 */
        return 0;

    seq_t distinct = 0;
    idx_t n_minus_1 = n - 1;        /* (n-1) as idx_t for arithmetic */

    for (idx_t m = 1; m <= n; ++m) {
        idx_t g1 = _gcd_idx(n, m);
        idx_t g2 = _gcd_idx(n, m + 1);   /* gcd(n, n+1) == 1 */

        seq_t numerator = (seq_t)n_minus_1 + (seq_t)g2 - (seq_t)g1;
        seq_t diff      = numerator >> 1;   /* (numerator)/2, always integer */

        size_t idx = (size_t)diff;          /* diff ∈ [0, n-1] */
        if (!seen[idx]) {
            seen[idx] = true;
            ++distinct;
        }
    }

    free(seen);
    return distinct;
}
