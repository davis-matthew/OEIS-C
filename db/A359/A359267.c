/*
 * A359267  Numbers k such that A359194(k) < k.
 * https://oeis.org/A359267
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n): n=1,2 -> 1,2; then for k>=2 the k-th run is 2^k ..
 *            2^k+A005578(k)-1, with A005578(0)=1,
 *            A005578(2j)=2*A005578(2j-1), A005578(2j+1)=2*A005578(2j)-1
 * Range:     u8 n <= 62, u16 n <= 62, u32 n <= 62, u64 n <= 62, u128 n <= 62,
 *            big any n
 * Validated: partial, pass, 62 terms through n = 62 (2026-11-10)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n)
{
    if (n == 1) return (seq_t)1;
    if (n == 2) return (seq_t)2;
    seq_t rem = (seq_t)(n - 2);
    seq_t v = 1;      /* A005578(k) */
    seq_t p = 1;      /* 2^k */
    int k;
    /* advance to k = 1 */
    v = v * 2 - 1;
    p = p * 2;
    for (k = 2; k < 400; k++) {
        if (k % 2 == 0) v = v * 2; else v = v * 2 - 1;
        p = p * 2;
        if (rem <= v) {
            return p + rem - 1;
        }
        rem -= v;
    }
    return (seq_t)0;
}
