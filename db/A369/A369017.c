/*
 * A369017  Triangle read by rows: T(n, k) = binomial(n-1, k-1) * (k - 1)^(k -
 *          1) * k * (n - k + 1)^(n - k - 1).
 * https://oeis.org/A369017
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   a(n) is the n-th term of the triangle read by rows where T(n, k)
 *            = binomial(n-1, k-1) * (k-1)^(k-1) * k * (n-k+1)^(n-k-1), with
 *            offsets starting at 0
 * Range:     u8 n <= 18, u16 n <= 33, u32 n <= 70, u64 n <= 164,
 *            u128 n <= 424, big any n
 * Helpers:   big_pow, binomial
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/binomial.h"
#include "../utils/big_pow.h"

/* Compute a single triangle entry T(n, k).  */
static seq_t T(idx_t n, idx_t k)
{
    if (k == 0) return 0;     /* column 0 is always 0 */
    if (n == 0) return 0;     /* row 0 only has column 0 */

    seq_t term = binomial(n - 1, k - 1);

    /* (k-1)^(k-1)  (with 0^0 = 1) */
    if (k > 1)
        term *= big_pow((seq_t)(k - 1), (seq_t)(k - 1));

    /* multiply by k */
    term *= (seq_t)k;

    /* (n-k+1)^(n-k-1); exponent -1 only occurs when n==k, where we treat the
       power as 1.  */
    if (n != k) {
        idx_t base = n - k + 1;
        idx_t exp  = n - k - 1;    /* exp >= 0 for n > k */
        term *= big_pow((seq_t)base, (seq_t)exp);
    }

    return term;
}

/* Find the row r such that r*(r+1)/2 <= idx < (r+1)*(r+2)/2.  */
static idx_t row_of(idx_t idx)
{
    idx_t low = 0, high = idx;
    while (low <= high) {
        idx_t mid = low + ((high - low) >> 1);
        __int128 tri = (__int128)mid * (mid + 1) / 2;
        if (tri <= idx)
            low = mid + 1;
        else
            high = mid - 1;
    }
    return high;   /* largest r with r*(r+1)/2 <= idx */
}

/* The required sequence function.  */
seq_t a(idx_t n)
{
    if (n == 0) return 0;   /* a(0) = 0 */

    idx_t row = row_of(n);
    idx_t before = (__int128)row * (row + 1) / 2;
    idx_t col = n - before;     /* zero‑based column in the row */

    return T(row, col);
}
