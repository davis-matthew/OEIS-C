/*
 * A244123  Triangle read by rows: terms T(n,k) of a binomial decomposition of
 *          n^n as Sum(k=0..n)T(n,k).
 * https://oeis.org/A244123
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign, tabl
 * Formula:   a(n) = T(r,k) where r = floor((sqrt(8*n+1)-1)/2) and k = n -
 *            r*(r+1)/2. For r>0, k>0, T(r,k) =
 *            r*(r+k)^(k-1)*(-k)^(r-k)*binomial(r,k). For k=0 (r>0) T=0, and
 *            for r=0 we return 1
 * Range:     i8 n <= 11, i16 n <= 18, i32 n <= 41, i64 n <= 112,
 *            i128 n <= 290, big any n
 * Helpers:   big_pow, binomial
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/binomial.h"
#include "../utils/big_pow.h"

seq_t a(idx_t n)
{
    /* Special case n = 0 -> 0^0 = 1 */
    if (n == 0)
        return (seq_t)1;

    /* Find row r: largest r such that r*(r+1)/2 <= n */
    idx_t low = 0, high = n + 1;
    while (low < high) {
        idx_t mid = low + (high - low + 1) / 2;
        __int128 val = (__int128)mid * (mid + 1) / 2;
        if (val <= ( __int128)n)
            low = mid;
        else
            high = mid - 1;
    }
    idx_t r = low;

    /* Index within the row */
    idx_t offset = n - r * (r + 1) / 2;
    idx_t k = offset;

    /* For r>0 and k==0, T is zero */
    if (k == 0)
        return (seq_t)0;

    seq_t seq_r = (seq_t)r;
    seq_t seq_k = (seq_t)k;

    /* Compute r * (r+k)^(k-1) */
    seq_t term = seq_r;
    if (k > 1) {
        term *= big_pow((seq_t)(r + k), (seq_t)(k - 1));
    }

    /* Compute (-k)^(r-k) */
    seq_t pow_k = big_pow(seq_k, (seq_t)(r - k));
    if ((r - k) & 1)
        pow_k = -pow_k;
    term *= pow_k;

    /* Multiply by binomial(r,k) */
    term *= binomial((idx_t)r, (idx_t)k);

    return term;
}
