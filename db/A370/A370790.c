/*
 * A370790  Lexicographically earliest sequence of distinct nonnegative
 *          integers such that for any n >= 0, a(n) AND a(n+1) equals 0, a(n)
 *          or a(n+1) (where AND denotes the bitwise AND operator).
 * https://oeis.org/A370790
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(0) = 0. For n ≥ 1 let k = ⌊log₂(n)⌋ and p = 2^k (the largest
 *            power of two ≤ n). Let m = n – p (0 ≤ m < p). The value a(n) is
 *            obtained by adding p to the k‑th element of the sequence f(k,·)
 *            where f(0,0)=0 and for k ≥ 1 f(k,m) = f(k‑1,m) if m < 2^{k‑1}
 *            f(k,m) = f(k‑1,(m‑2^{k‑1}+2^{k‑1}‑1) mod 2^{...
 * Range:     u8 n <= 255, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Validated: standard, pass, 84 terms through n = 9239 (2026-11-01)
 * Helpers:   ilog2
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdint.h>
#include "../utils/ilog2.h"

static seq_t order_value(unsigned k, idx_t offset)
{
    seq_t add = 0;
    idx_t m   = offset;
    idx_t size = ((idx_t)1) << k;        /* 2^k */

    while (size > 1) {
        idx_t half = size >> 1;          /* 2^{k‑1} */
        if (m < half) {
            size = half;
        } else {
            m -= half;                   /* index inside second half */
            add += half;                 /* set the high bit */
            /* rotate right by one inside the second half */
            if (m == 0) {
                m = half - 1;
            } else {
                --m;
            }
            size = half;
        }
    }
    return add;                          /* f(k,offset) */
}

seq_t a(idx_t n)
{
    if (n == 0) return 0;

    unsigned k = ilog2(n);
    idx_t p    = ((idx_t)1) << k;         /* 2^k */
    idx_t offset = n - p;                 /* 0 ≤ offset < p */
    seq_t m  = order_value(k, offset);
    return (seq_t)(p + m);
}
