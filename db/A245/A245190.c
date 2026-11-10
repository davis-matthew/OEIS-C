/*
 * A245190  Products of successive blocks of 2^n terms of A245180.
 * https://oeis.org/A245190
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Product_{k=2^n..2^(n+1)-1} A245180(k), where A245180(k) =
 *            8^(r-1) * Product R(i) over the r runs of 1's in binary k, with
 *            run lengths i, R(1)=1, R(2)=3, R(i) = 4*R(i-1) - (-2)^(i-2)
 * Range:     u8 n <= 1, u16 n <= 2, u32 n <= 3, u64 n <= 3, u128 n <= 4,
 *            big any n
 * Validated: partial, pass, 6 terms through n = 5 (2026-11-07)
 * Helpers:   mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>
#include "../utils/mem.h"

seq_t a(idx_t n)
{
    idx_t m = n + 2;
    seq_t *R = mem_new(seq_t, m + 1);
    R[1] = (seq_t)1;
    seq_t p2 = (seq_t)1; /* 2^(i-2) */
    for (idx_t i = 2; i <= m; ++i) {
        if (i > 2) p2 = p2 * (seq_t)2;
        if (i & 1) R[i] = (seq_t)4 * R[i - 1] + p2; /* (-2)^(i-2) negative when i odd */
        else       R[i] = (seq_t)4 * R[i - 1] - p2;
    }
    seq_t total = (seq_t)1;
    idx_t lo = ((idx_t)1) << n;
    idx_t hi = (((idx_t)1) << (n + 1)) - 1;
    for (idx_t k = lo; ; ++k) {
        idx_t t = k, len = 0;
        bool first = true;
        seq_t prod = (seq_t)1;
        while (t > 0) {
            if (t & 1) {
                ++len;
            } else if (len > 0) {
                prod *= R[len];
                if (!first) prod *= (seq_t)8;
                first = false;
                len = 0;
            }
            t >>= 1;
        }
        if (len > 0) {
            prod *= R[len];
            if (!first) prod *= (seq_t)8;
        }
        total *= prod;
        if (k == hi) break;
    }
    mem_free(R);
    return total;
}
