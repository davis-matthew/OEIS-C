/*
 * A308350  Number of maximum irredundant sets in the n-web graph.
 * https://oeis.org/A308350
 *
 * Offset:    3    (a(3) is the first term)
 * Keywords:  nonn, easy
 * Formula:   Piecewise: * a(4*k) = 6 * 4^k * a(4*k+1) = 9 * (4*k+1) * (2*k+1)
 *            * 4^(k-1) (k>0) * a(4*k+2) = 4 * 4^k * a(4*k+3) = (4*k+3) *
 *            (9*k+7) * 4^k
 * Range:     u8 n <= 6, u16 n <= 16, u32 n <= 42, u64 n <= 102, u128 n <= 226,
 *            big any n
 * Helpers:   pow_uint
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/pow_uint.h"

seq_t a(idx_t n)
{
    /* Behavior is defined for n ≥ 3 (offset 3).  Below that we return 0. */
    if (n < 3) return 0;

    idx_t k = n / 4;
    idx_t r = n % 4;

    switch (r) {
        case 0: {                                 /* a(4*k) */
            seq_t p = pow_uint(4, k);
            return (seq_t)6 * p;
        }
        case 1: {                                 /* a(4*k+1) */
            seq_t t1 = (seq_t)(4 * k + 1);
            seq_t t2 = (seq_t)(2 * k + 1);
            seq_t p  = pow_uint(4, k - 1);       /* k ≥ 1 because n ≥ 3 */
            seq_t res = p;
            res *= t2;
            res *= t1;
            res *= (seq_t)9;
            return res;
        }
        case 2: {                                 /* a(4*k+2) */
            seq_t p = pow_uint(4, k);
            return (seq_t)4 * p;
        }
        case 3: {                                 /* a(4*k+3) */
            seq_t t1 = (seq_t)(4 * k + 3);
            seq_t t2 = (seq_t)(9 * k + 7);
            seq_t p  = pow_uint(4, k);
            seq_t res = p;
            res *= t2;
            res *= t1;
            return res;
        }
    }
    /* Unreachable, but GCC may warn. */
    return 0;
}
