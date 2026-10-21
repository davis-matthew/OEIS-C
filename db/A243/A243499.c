/*
 * A243499  Product of parts of integer partitions as enumerated in the table
 *          A125106.
 * https://oeis.org/A243499
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, look
 * Formula:   a(0) = 1; a(2n+1) = a(n); a(2n) = (2 + v₂(n)) * a(n -
 *            2^{v₂(n)}), n ≥ 1 where v₂(k) is the exponent of 2 in k (number
 *            of trailing zeros)
 * Range:     u8 n <= 119, u16 n <= 2031, u32 n <= 8191, u64 n <= 8191,
 *            u128 n <= 8191, big any n
 * Helpers:   mul_small
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* The sequence is computed recursively; no memoisation is required. */

#include <stdint.h>
#include "../utils/mul_small.h"

/* Return the exponent of 2 in x (0 only when x==0, which never
   occurs in this function).  */
static unsigned int v2(idx_t x) {
    unsigned int cnt = 0;
    while ((x & 1U) == 0) {
        x >>= 1;
        cnt++;
    }
    return cnt;
}

/* Public function required by the exercise. */
seq_t a(idx_t n) {
    if (n == 0U) {
        return (seq_t)1;
    }

    /* Odd index: a(2n+1) = a(n) */
    if ((n & 1U) != 0U) {
        return a((n - 1U) >> 1);
    }

    /* Even index: a(2n) = (2 + v₂(n)) * a(n - 2^{v₂(n)}) */
    idx_t m = n >> 1;          /* n = 2*m  */
    unsigned int f = v2(m);    /* exponent of 2 in m */
    idx_t sub = m - ((idx_t)1 << f); /* m - 2^f */
    seq_t sub_val = a(sub);
    /* Multiply by the small integer (2 + f). */
    return mul_small(sub_val, (unsigned int)(2 + f));
}
