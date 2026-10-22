/*
 * A383910  Expansion of Product_{k=0..3} (1 + k*x)/(1 - k*x).
 * https://oeis.org/A383910
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(0) = 1; a(n) = 20*3^n - 15*2^(n+1) + 12 for n > 0 (derived
 *            from A383912: 20*3^n - 15*2^(n+1) + 12)
 * Range:     u8 n <= 2, u16 n <= 7, u32 n <= 17, u64 n <= 26, u128 n <= 26,
 *            big any n
 * Helpers:   pow2, pow3
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/pow2.h"
#include "../utils/pow3.h"

seq_t a(idx_t n)
{
    if (n == 0) {
        return (seq_t)1;
    }

    /* Compute 3^n and 2^(n+1) using the provided helpers */
    seq_t pow3_n = pow3(n);          /* 3^n */
    seq_t pow2_n1 = pow2(n + 1);    /* 2^(n+1) */

    /* a(n) = 20*pow3_n - 15*pow2_n1 + 12 */
    seq_t result = 20 * pow3_n;
    result -= 15 * pow2_n1;
    result += 12;

    return result;
}
