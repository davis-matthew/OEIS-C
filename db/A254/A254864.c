/*
 * A254864  Triangular table T(n,k) = n! / (n-floor(n/3^k))!, read by rows
 *          T(1,1), T(2,1), T(2,2), T(3,1), T(3,2), T(3,3), ...
 * https://oeis.org/A254864
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   a(n) = ∏_{m = (r – ⌊r/3^k⌋ + 1)}^r m where r and k are the
 *            row–column indices of position n in the triangular table T(r,k)
 *            = r! / (r – ⌊r / 3^k⌋)!
 * Range:     u8 n <= 36, u16 n <= 105, u32 n <= 276, u64 n <= 741,
 *            u128 n <= 2278, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n)
{
    /* The sequence is defined only for n ≥ 1.  For safety return 0 if n == 0. */
    if (n == 0) return (seq_t)0;

    /* --- Find the row r such that the first r rows contain at least n terms.
       The number of terms up to row r is r(r+1)/2.  Binary search in the range
       [1, n] suffices because r ≤ n.  Use unsigned __int128 to avoid overflow. */
    idx_t low = 1, high = n;
    while (low < high) {
        idx_t mid = low + (high - low) / 2;
        unsigned __int128 sum = (unsigned __int128)mid * (mid + 1) / 2;
        if (sum >= n) high = mid;
        else          low  = mid + 1;
    }
    idx_t r = low;

    /* --- Column index k within row r. */
    unsigned __int128 prev_sum = (unsigned __int128)(r - 1) * r / 2;
    idx_t k = n - (idx_t)prev_sum;      /* 1‑based column */

    /* --- Compute div = ⌊r / 3^k⌋ by repeated division.  Once div becomes 0,
           it stays 0, yielding T(r,k)=1. */
    idx_t div = r;
    for (idx_t i = 1; i <= k; ++i) {
        div /= 3;
        if (div == 0) break;
    }
    if (div == 0) return (seq_t)1;

    /* --- Product from (r – div + 1) to r inclusive. */
    idx_t start = r - div + 1;
    seq_t result = (seq_t)1;
    for (idx_t x = start; x <= r; ++x) {
        result *= (seq_t)x;
    }
    return result;
}
