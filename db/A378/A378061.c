/*
 * A378061  Triangle read by rows: T(n, k) = binomial(n + 1, (n - k)/2)^2*(k +
 *          1)/(n + 1) if n - k is even, otherwise 0.
 * https://oeis.org/A378061
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   not stated
 * Range:     u8 n <= 15, u16 n <= 45, u32 n <= 65, u64 n <= 65, u128 n <= 65,
 *            big any n
 * Helpers:   binomial, floor_sqrt
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*
 * Formula: T(n, k) = binomial(n + 1, (n - k)/2)^2 * (k + 1) / (n + 1)   if n – k is even
 *                 0                                            otherwise
 *
 * The sequence a(n) is the row‐sum Σk T(n,k).  This is equivalent to
 * enumerating all pairs (n,k) where 0 ≤ k ≤ n and n–k is even and
 * adding the corresponding triangle values.  The implementation below
 * computes a(n) directly from the definition using the helper
 * functions `binomial` (in ../utils/binomial.h) and
 * `floor_sqrt` (in ../utils/floor_sqrt.h).  To avoid overflow in the
 * row‑index calculation even for small seq_t types, the row is found
 * by an O(√n) loop.
 */

#include <stdint.h>
#include "../utils/binomial.h"
#include "../utils/floor_sqrt.h"

/* external linkage required */
seq_t a(idx_t n)
{
    /* ---- Find row r and column c for index n in the flattened triangle ---- */
    idx_t r = 0;          /* current row */
    idx_t sum = 0;        /* total number of elements in rows < r */

    while (sum + (r + 1) <= n) {
        sum += r + 1;
        r++;
    }
    idx_t c = n - sum;    /* column index within row r */

    /* ---- Compute the value T(r, c) ---- */
    seq_t res = 0;
    if (((r - c) & 1) == 0) {              /* only even difference contributes */
        idx_t m = (r - c) / 2;            /* (n - k)/2  */
        seq_t b = binomial(r + 1, m);     /* binomial(r+1, m) */
        res = b * b;                      /* square of binomial */
        res = res * (c + 1);              /* multiply by (k+1) */
        res = res / (r + 1);              /* divide by (n+1) */
    }
    return res;
}
