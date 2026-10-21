/*
 * A273317  Irregular table read by rows: T(0,0) = 2 and T(n,2k) = T(n-1,k)+1,
 *          T(n,2k+1) = T(n-1,k)*(T(n-1,k)+1) for 0 <= k < 2^(n-1).
 * https://oeis.org/A273317
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, tabf
 * Formula:   a(n) is read row by row from the irregular table defined by
 *            T(0,0)=2 and for n≥1, T(n,2k) = T(n-1,k) + 1, T(n,2k+1) =
 *            T(n-1,k) * (T(n-1,k) + 1). The function computes a(n) using the
 *            binary representation of n+1
 * Range:     u8 n <= 13, u16 n <= 29, u32 n <= 54, u64 n <= 54, u128 n <= 54,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n)
{
    /* find the row: r = floor(log2(n+1)) */
    idx_t v = n + 1;
    idx_t r = 0;
    while (v >>= 1) ++r;

    /* offset where this row starts: 2^r - 1 */
    idx_t offset = ((uint64_t)1 << r) - 1;
    idx_t k = n - offset;     /* position within the row */

    seq_t cur = (seq_t)2;
    /* process the r bits of k from most significant to least */
    for (idx_t i = r; i-- > 0; ) {
        if (((k >> i) & 1ULL) == 0) {
            cur = cur + 1;
        } else {
            cur = cur * (cur + 1);
        }
    }
    return cur;
}
