/*
 * A242653  Triangle read by rows: T(n,k) = ((n+k)/2)!/k! if n,k have same
 *          parity, otherwise 0.
 * https://oeis.org/A242653
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, tabl, easy
 * Formula:   Let r = ⌊(√(8n+1) – 1)/2⌋ be the row index, let s = r(r+1)/2 be
 *            the starting index of that row, let k = n – s be the column
 *            index. If (r + k) is odd, a(n) = 0. Otherwise let m = (r + k)/2
 *            and a(n) = ∏_{i=k+1}^{m} i (product of integers from k+1 to m)
 * Range:     u8 n <= 56, u16 n <= 137, u32 n <= 325, u64 n <= 821,
 *            u128 n <= 2214, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
static inline unsigned __int128 _tri_u64(uint64_t t) {
    return (unsigned __int128)t * (t + 1) / 2;
}
seq_t a(idx_t n) {
    /* locate the row r such that  r(r+1)/2 ≤ n < (r+1)(r+2)/2 */
    idx_t low = 0;
    idx_t high = 1;
    while (_tri_u64(high) <= n)
        high <<= 1;
    while (low < high) {
        idx_t mid = (low + high + 1) >> 1;
        if (_tri_u64(mid) <= n)
            low = mid;
        else
            high = mid - 1;
    }
    idx_t r = low;
    idx_t start = (idx_t)_tri_u64(r);
    idx_t k = n - start;
    if (((r + k) & 1) != 0)
        return (seq_t)0;
    idx_t m = (r + k) >> 1;
    seq_t result = 1;
    for (idx_t i = k + 1; i <= m; ++i)
        result *= (seq_t)i;
    return result;
}
