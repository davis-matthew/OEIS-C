/*
 * A248336  Smallest number m such that the Levenshtein distance of m and its
 *          reversal equals n.
 * https://oeis.org/A248336
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(0) = 0 a(2*k) = 10^(2*k‑1) + 2*(10^k‑1)/9 for k >= 1 a(2*k+1)
 *            = 10^(2*k+1) for all k >= 0 (based on OEIS A248336 formulas)
 * Range:     u8 n <= 2, u16 n <= 4, u32 n <= 10, u64 n <= 19, u128 n <= 19,
 *            big any n
 * Helpers:   pow10
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/pow10.h"

seq_t a(idx_t n)
{
    if (n == 0)
        return 0;                     /* base case */

    if (n & 1U) {                    /* odd n = 2*k+1 */
        idx_t k = n / 2;
        return pow10(2 * k + 1);
    } else {                         /* even n = 2*k, k >= 1 */
        idx_t k = n / 2;
        seq_t pow10_k = pow10(k);                    /* 10^k */
        seq_t mult = (pow10_k - 1) / 9;              /* (10^k-1)/9 */
        mult = mult * 2;                            /* 2 * that */
        seq_t pow10_2k_1 = pow10(2 * k - 1);         /* 10^(2*k-1) */
        return pow10_2k_1 + mult;                   /* final value */
    }
}
