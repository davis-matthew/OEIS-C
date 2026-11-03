/*
 * A258122  The multiplicative Wiener index of the cycle graph C_n (n>=3).
 * https://oeis.org/A258122
 *
 * Offset:    3    (a(3) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = (k!)^n if n = 2k + 1 is odd; a(n) = k^k*((k-1)!)^n if n =
 *            2k is even
 * Range:     u8 n <= 5, u16 n <= 6, u32 n <= 8, u64 n <= 10, u128 n <= 13,
 *            big any n
 * Validated: partial, pass, 15 terms through n = 17 (2026-11-01)
 * Helpers:   ipow_i32
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/ipow_i32.h"

/* Helper: compute factorials iteratively.
   For built‑in integer types the result may overflow for large n,
   but the tests use only n for which the value fits in seq_t. */
static seq_t factorial(idx_t n)
{
    seq_t result = 1;
    for (idx_t i = 2; i <= n; ++i)
        result *= (seq_t)i;
    return result;
}

seq_t a(idx_t n)
{
    if (n < 3) return 0;          /* Offset is 3 – behaviour for n < 3 does not matter */
    if ((n & 1) == 1) {           /* n is odd: n = 2k + 1  (k >= 1) */
        idx_t k = (n - 1) >> 1;
        seq_t fact_k = factorial(k);          /* k!  */
        return ipow_i32(fact_k, (int)n);       /* (k!)^n  */
    } else {                       /* n is even: n = 2k  (k >= 2) */
        idx_t k = n >> 1;
        seq_t pow1 = ipow_i32((seq_t)k, (int)k);       /* k^k  */
        seq_t fact_k1 = factorial(k - 1);            /* (k-1)!  */
        seq_t pow2 = ipow_i32(fact_k1, (int)n);       /* ((k-1)!)^n  */
        return pow1 * pow2;                         /* product */
    }
}
