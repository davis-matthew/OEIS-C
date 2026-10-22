/*
 * A317506  Triangle read by rows: T(0,0) = 1; T(n,k) = 2 T(n-1,k) -
 *          T(n-4,k-1) for 0 <= k <= floor(n/4); T(n,k)=0 for n or k < 0.
 * https://oeis.org/A317506
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  tabf, sign, easy
 * Formula:   T(n,k) = (-1)^k * 2^(n-4k) * C(n-3k, k) a(n) is obtained by
 *            reading T(n,k) row-wise: for each n ≥ 0, k = 0 .. ⌊n/4⌋
 * Range:     i8 n <= 9, i16 n <= 35, i32 n <= 49, i64 n <= 49, i128 n <= 49,
 *            big any n
 * Helpers:   binomial, pow2
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>
#include "../utils/binomial.h"   /* seq_t binomial(idx_t n, idx_t k); */
#include "../utils/pow2.h"       /* seq_t pow2(idx_t exp); */

/* cumulative number of terms up to row n (inclusive of row n) */
static idx_t cumulative_terms(idx_t n)
{
    idx_t m = n / 4;
    idx_t r = n % 4;
    /* S(n) = 2*m*(m+1) + (r+1)*(m+1) */
    return 2 * m * (m + 1) + (r + 1) * (m + 1);
}

seq_t a(idx_t n)
{
    idx_t low = 0, high = 1;

    /* find an upper bound for the row containing n */
    while (cumulative_terms(high) <= n)
        high <<= 1;                 /* double until we pass the index */

    /* binary search for smallest row r with cumulative_terms(r) > n */
    while (low < high) {
        idx_t mid = low + (high - low) / 2;
        if (cumulative_terms(mid) > n)
            high = mid;
        else
            low = mid + 1;
    }
    idx_t row = high;                /* minimal n with cumulative_terms(row) > n */

    idx_t prevS = (row == 0) ? 0ULL : cumulative_terms(row - 1);
    idx_t k = n - prevS;             /* position within the row */

    /* compute T(row, k) = (-1)^k * 2^(row-4k) * C(row-3k, k) */
    seq_t val = binomial(row - 3 * k, k);
    if (k & 1)               /* k is odd */
        val = -val;
    val = val * pow2(row - 4 * k);
    return val;
}
