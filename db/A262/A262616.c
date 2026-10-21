/*
 * A262616  Triangle read by rows: T(n,k) = 4^(n-k), n>=0, 0<=k<=n.
 * https://oeis.org/A262616
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, tabl, easy
 * Formula:   a(n) = 4^( r - ( n - r*(r+1)/2 ) ), where r = floor( ( √(8n+1) -
 *            1 ) / 2 )
 * Range:     u8 n <= 9, u16 n <= 35, u32 n <= 135, u64 n <= 527,
 *            u128 n <= 2079, big any n
 * Helpers:   pow4, tri_prod
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  Implements the triangle read by rows:
 *  T(n,k) = 4^(n-k),  0 ≤ k ≤ n  (rows are flattened row‑by‑row).   */
#include <stdint.h>
#include <stdbool.h>
#include "../utils/pow4.h"
#include "../utils/tri_prod.h"

/* Compute a(n) for the required triangle */
seq_t a(idx_t n)
{
    /* Find r such that tri_prod(r) <= n < tri_prod(r+1) */
    idx_t low = 0, high = 1;

    /* Expand high until it exceeds the required r */
    while (tri_prod(high) <= n) {
        if (high > UINT64_MAX / 2) {   /* prevent wrap‑around */
            high = UINT64_MAX;
            break;
        }
        high <<= 1;
    }

    /* Binary search for the largest r satisfying tri_prod(r) <= n */
    while (low < high) {
        idx_t mid = low + (high - low + 1) / 2;
        if (tri_prod(mid) <= n)
            low = mid;
        else
            high = mid - 1;
    }
    idx_t r = low;

    /* Position within the row (0‑based) */
    idx_t start = (idx_t)(tri_prod(r));
    idx_t pos   = n - start;

    /* Exponent of 4 in the answer */
    idx_t exp   = r - pos;

    return pow4(exp);
}
