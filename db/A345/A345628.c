/*
 * A345628  Irregular triangle T(n,k) read by rows of the number of elements
 *          of order k in the dicyclic group Dic(n) for n>=2.
 * https://oeis.org/A345628
 *
 * Offset:    2    (a(2) is the first term)
 * Keywords:  nonn, tabf
 * Formula:   a(n) is the flattened count of elements of order k ** in the
 *            dicyclic group Dic(row), where row is chosen so that **
 *            start(row) = row*(row-1) ≤ n < row*(row+1). ** For each row = r
 *            (r ≥ 2), a^i elements contribute φ(k) when ** k | 2r; and the
 *            a^i x part contributes 2r elements of order 4
 * Range:     u8 n <= 8700, u16 n <= 8700, u32 n <= 8700, u64 n <= 8700,
 *            u128 n <= 8700, big any n
 * Helpers:   phi
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stddef.h>
#include "../utils/phi.h"

/* Integer (64‑bit) square root by binary search */
static inline idx_t isqrt_u64(idx_t n)
{
    idx_t low = 0, high = n;
    while (low <= high) {
        idx_t mid = low + ((high - low) >> 1);
        __int128 sq = (__int128)mid * mid;
        if (sq <= n) {
            if (mid == n) return mid;
            low = mid + 1;
        } else {
            if (mid == 0) break;
            high = mid - 1;
        }
    }
    return high;
}

seq_t a(idx_t n)
{
    if (n < 2) return 0;

    /*-------------------- find the row number --------------------*/
    __int128 tmp = (__int128)1 + (__int128)4 * n;
    idx_t val   = (idx_t)tmp;                     /* fits for normal indices */
    idx_t sqrtv = isqrt_u64(val);
    idx_t row   = (1 + sqrtv) / 2;                /* row = r */

    /* start index of the row: start(row) = row*(row-1) */
    __int128 start128 = (__int128)row * (row - 1);
    idx_t      start  = (idx_t)start128;

    /* column inside the row (1‑based) */
    idx_t k = n - start + 1;        /* 1 ≤ k ≤ 2*row */

    /*-------------------- compute the count --------------------*/
    seq_t result = 0;
    idx_t m = 2 * row;              /* |⟨a⟩| = 2*row */

    /* contribution from a^i elements */
    if ((m % k) == 0) {
        result += (seq_t)phi(k);   /* φ(k) fits in seq_t for k ≤ m */
    }

    /* contribution from a^i x elements (always of order 4) */
    if (k == 4) {
        result += (seq_t)((__int128)m);   /* 2*row elements */
    }

    return result;
}
