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
 * Helpers:   floor_log2_uint64, order_value
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/floor_log2_uint64.h"
#include "../utils/order_value.h"

seq_t a(idx_t n)
{
    if (n == 0) return 0;

    unsigned k = floor_log2_uint64(n);
    idx_t p    = ((idx_t)1) << k;         /* 2^k */
    idx_t offset = n - p;                 /* 0 ≤ offset < p */
    seq_t m  = order_value(k, offset);
    return (seq_t)(p + m);
}
