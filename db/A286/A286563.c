/*
 * A286563  Triangular table T(n,k) read by rows: T(n,1) = 1, and for 1 < k <=
 *          n, T(n,k) = the highest exponent e such that k^e divides n.
 * https://oeis.org/A286563
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   a(n) is the sequence obtained by reading the triangular table
 *            T(row,k) row‑wise. T(row,1) = 1 For k>1, T(row,k) = max e≥0 such
 *            that k^e divides row
 * Range:     u8 n <= 8078, u16 n <= 8078, u32 n <= 8078, u64 n <= 8078,
 *            u128 n <= 8078, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>

static inline unsigned __int128 tri128(idx_t r) {
    return (unsigned __int128)r * (r + 1) / 2;
}

static inline idx_t row_of_idx(idx_t n) {
    idx_t low = 1, high = n;
    while (low < high) {
        idx_t mid = (low + high) >> 1;
        if (tri128(mid) >= n)
            high = mid;
        else
            low = mid + 1;
    }
    return low;
}

seq_t a(idx_t n) {
    idx_t row = row_of_idx(n);
    idx_t prefix = (idx_t)tri128(row - 1);
    idx_t k = n - prefix;
    if (k == 1) return (seq_t)1;
    seq_t e = 0;
    idx_t temp = row;
    while (temp % k == 0) {
        temp /= k;
        e++;
    }
    return e;
}
