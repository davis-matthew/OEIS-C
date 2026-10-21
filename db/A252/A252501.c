/*
 * A252501  Triangle T read by rows: T(n,k) = binomial(2*n+1,k)*binomial(n,k),
 *          n>=0, 0<=k<=n.
 * https://oeis.org/A252501
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   a(n) = binomial(2*r+1,k) * binomial(r,k) where (r,k) are the row
 *            and column indices of n in row‑major order
 * Range:     u8 n <= 12, u16 n <= 39, u32 n <= 44, u64 n <= 44, u128 n <= 44,
 *            big any n
 * Helpers:   _start, binomial
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/binomial.h"
#include "../utils/_start.h"

/* find the row r such that _start(r) <= n < _start(r+1) */
static idx_t find_row(idx_t n) {
    idx_t low = 0, high = 1;
    /* grow high until start(high) > n */
    while (_start(high) <= (unsigned __int128)n) {
        if (high > ((idx_t)-1) >> 1)
            break;
        high <<= 1;
    }
    low = high >> 1;
    /* binary search */
    while (low < high) {
        idx_t mid = low + ((high - low) >> 1);
        if (_start(mid) <= (unsigned __int128)n)
            low = mid + 1;
        else
            high = mid;
    }
    return low - 1;  /* last r with start(r) <= n */
}

seq_t a(idx_t n) {
    idx_t r = find_row(n);
    idx_t start_r = (idx_t)_start(r);
    idx_t k = n - start_r;
    seq_t b1 = binomial(2 * r + 1, k);
    seq_t b2 = binomial(r, k);
    return b1 * b2;
}
