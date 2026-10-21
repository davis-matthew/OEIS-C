/*
 * A272263  a(n) = numerator of A000032(n) - 1/2^n.
 * https://oeis.org/A272263
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, frac, easy
 * Formula:   a(n) = Lucas(n)*2^n - 1 * where Lucas(n) is the Lucas numbers:
 *            L(0)=2, L(1)=1, L(n)=L(n-1)+L(n-2) for n≥2
 * Range:     u8 n <= 4, u16 n <= 9, u32 n <= 18, u64 n <= 37, u128 n <= 75,
 *            big any n
 * Helpers:   pow2
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include "../utils/pow2.h"  /* pow2(idx_t exp) -> seq_t */

seq_t
a(idx_t n)
{
    /* Lucas numbers base cases */
    seq_t l_prev = (seq_t)2;  /* L(0) */
    seq_t l_curr = (seq_t)1;  /* L(1) */

    /* Handle small n directly */
    if (n == 0) {
        seq_t res = l_prev * pow2(n);
        res -= (seq_t)1;
        return res;
    }
    if (n == 1) {
        seq_t res = l_curr * pow2(n);
        res -= (seq_t)1;
        return res;
    }

    /* Compute Lucas(n) iteratively for n ≥ 2 */
    idx_t i;
    for (i = 2; i <= n; ++i) {
        seq_t l_next = l_curr + l_prev;
        l_prev = l_curr;
        l_curr = l_next;
    }

    /* a(n) = Lucas(n) * 2^n - 1 */
    seq_t result = l_curr * pow2(n);
    result -= (seq_t)1;
    return result;
}
