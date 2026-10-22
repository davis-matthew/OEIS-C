/*
 * A375854  Triangle read by rows: T(n, k) = 2^k * hypergeom([-n, -k], [],
 *          1/2).
 * https://oeis.org/A375854
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   a(n) enumerates the triangle T(r,k) read row‑wise * T(r,k) =
 *            Σ_{j=0..k} 2^(k-j) * binomial(r,j) * binomial(k,j) * j!
 * Range:     u8 n <= 13, u16 n <= 33, u32 n <= 54, u64 n <= 54, u128 n <= 54,
 *            big any n
 * Helpers:   binomial, factorial, pow2
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/pow2.h"
#include "../utils/binomial.h"
#include "../utils/factorial.h"
#include <stdint.h>
#include <stddef.h>

seq_t a(idx_t n)
{
    /* Find the row r with r(r+1)/2 <= n < (r+1)(r+2)/2. */
    idx_t r = 0;
    idx_t low = 0;
    idx_t high = n;             /* inclusive upper bound */

    while (low <= high) {
        idx_t mid = low + ((high - low) >> 1);
        unsigned __int128 prod = (unsigned __int128)mid * (mid + 1) / 2;
        if (prod <= n) {
            r = mid;
            low = mid + 1;
        } else {
            if (mid == 0) break;
            high = mid - 1;
        }
    }

    /* Start of row r */
    unsigned __int128 offset128 = (unsigned __int128)r * (r + 1) / 2;
    idx_t offset = (idx_t)offset128;      /* offset <= n */
    idx_t k = n - offset;                 /* column within the row */

    seq_t sum = 0;
    for (idx_t j = 0; j <= k; ++j) {
        seq_t term = pow2(k - j);
        term = term * binomial(r, j);
        term = term * binomial(k, j);
        term = term * factorial(j);
        sum += term;
    }
    return sum;
}
