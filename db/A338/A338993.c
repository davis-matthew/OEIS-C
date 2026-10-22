/*
 * A338993  Triangle read by rows: T(n,k) is the number of k-permutations of
 *          {1,...,n} that form a nontrivial arithmetic progression, 1 <= k <=
 *          n.
 * https://oeis.org/A338993
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   a(n) = T(r,k) where r = min{ integer r > 0 | r(r+1)/2 >= n } k =
 *            n - r(r-1)/2 and T(r,k) = r if k==1 2*r*f - (k-1)*(f^2+f) if k>1
 *            with f = floor((r-1)/(k-1))
 * Range:     u8 n <= 78, u16 n <= 78, u32 n <= 78, u64 n <= 78, u128 n <= 78,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stddef.h>

/* Find the row number corresponding to index n. */
static inline idx_t _row_of(idx_t n) {
    /* binary search for minimal r with r*(r+1)/2 >= n */
    idx_t low = 1, high = n;
    while (low < high) {
        idx_t mid = low + (high - low) / 2;
        unsigned __int128 prod = (unsigned __int128)mid * (mid + 1) / 2;
        if (prod >= n)
            high = mid;
        else
            low = mid + 1;
    }
    return low;
}

seq_t a(idx_t n) {
    if (n == 0) return (seq_t)0;          /* safety, though offset is 1 */
    idx_t r = _row_of(n);
    unsigned __int128 before = (unsigned __int128)(r - 1) * r / 2;
    idx_t k = (idx_t)((unsigned __int128)n - before);

    if (k == 1)
        return (seq_t)r;

    idx_t f = (r - 1) / (k - 1);
    seq_t sr = (seq_t)r;
    seq_t sf = (seq_t)f;
    seq_t sk1 = (seq_t)(k - 1);
    seq_t res = (seq_t)2 * sr * sf - sk1 * (sf * sf + sf);
    return res;
}
