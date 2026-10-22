/*
 * A361948  Array read by ascending antidiagonals. A(n, k) =
 *          Product_{j=0..k-1} binomial((j + 1)*n - 1, n - 1) if n >= 1, and
 *          A(0, k) = 1 for all k.
 * https://oeis.org/A361948
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   not stated
 * Range:     u8 n <= 23, u16 n <= 38, u32 n <= 54, u64 n <= 54, u128 n <= 54,
 *            big any n
 * Helpers:   binomial
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* OEIS A361948
   Formula:  a(n) is obtained by reading the array
   A(n,k) = Π_{j=0}^{k-1} binomial((j+1)*n-1, n-1),  with A(0,k)=1
   along ascending antidiagonals n+k = const.  Within each diagonal the
   entries are taken in order of decreasing n (i.e., increasing k).
   This implementation directly translates an index n into its
   (n',k) coordinates and evaluates A(n',k). */

#include <stdint.h>
#include "../utils/binomial.h"

seq_t a(idx_t n)
{
    /* find the diagonal s such that cumulative count > n */
    idx_t s = 0;
    __int128 cum_128 = 1;                /* cumulative count for diagonal s */
    while (cum_128 <= (__int128)n) {
        ++s;
        cum_128 = (__int128)(s + 1) * (s + 2) / 2;
    }

    /* previous cumulative count (just before diagonal s) */
    __int128 prev_128 = (__int128)s * (s + 1) / 2;
    idx_t prev = (idx_t)prev_128;

    /* position within the diagonal:      offset (0 <= offset <= s) */
    idx_t offset = n - prev;

    /* coordinates inside the array */
    idx_t np = s - offset;   /* the row index n' */
    idx_t k  = offset;       /* the column index k  */

    /* Special cases: A(0,k) = 1, and A(n,0) = 1 */
    if (np == 0) {
        return (seq_t)1;
    }
    if (k == 0) {
        return (seq_t)1;
    }

    /* compute the product Π_{j=0}^{k-1} binomial((j+1)*np-1, np-1) */
    seq_t result = (seq_t)1;
    for (idx_t j = 0; j < k; ++j) {
        idx_t num = (j + 1) * np - 1;   /* (j+1)*np - 1 */
        idx_t den = np - 1;             /* np - 1 */
        seq_t term = binomial(num, den);
        result *= term;
    }
    return result;
}
