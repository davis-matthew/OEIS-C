/*
 * A366109  a(n) = floor(n!*(3*floor(n/2)!*ceiling(n/2)! +
 *          3*floor((n+2)/2)!*ceiling((n-2)/2)! -
 *          6*floor(n/2)!*ceiling((n-2)/2)!)^(-1)).
 * https://oeis.org/A366109
 *
 * Offset:    3    (a(3) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = floor( n! * ( 3*floor(n/2)!*ceil(n/2)! +
 *            3*floor((n+2)/2)!*ceil((n-2)/2)! - 6*floor(n/2)!*ceil((n-2)/2)!
 *            )^(-1) ) (offset 3)
 * Range:     u8 n <= 8, u16 n <= 16, u32 n <= 30, u64 n <= 39, u128 n <= 39,
 *            big any n
 * Helpers:   binomial
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/binomial.h"

seq_t a(idx_t n)
{
    if (n < 3) return 0;          /* behavior for n < 3 is irrelevant */
    if ((n & 1ULL) == 0) {        /* even n: n = 2k */
        idx_t k = n >> 1;
        seq_t bin = binomial(2*k - 2, k - 1);   /* C(2k-2,k-1) */
        return (bin * 2) / 3;         /* floor(2/3 * bin) */
    } else {                      /* odd n: n = 2k+1 */
        idx_t k = (n - 1) >> 1;
        seq_t bin = binomial(2*k, k);            /* C(2k,k) */
        seq_t num = bin * (seq_t)(2*k + 1);      /* (2k+1)*bin */
        return num / (seq_t)(6*k);              /* floor((2k+1)*bin/(6k)) */
    }
}
