/*
 * A308755  a(n) = Sum_{d|n} d^(d-2).
 * https://oeis.org/A308755
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Sum_{d|n} d^(d-2) (OEIS A308755)
 * Range:     u8 n <= 5, u16 n <= 7, u32 n <= 11, u64 n <= 17, u128 n <= 28,
 *            big any n
 * Helpers:   big_pow
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <string.h>
#include "../utils/big_pow.h"

seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0;   /* undefined, but safe */

    seq_t sum = (seq_t)0;

    for (idx_t i = 1; i * i <= n; ++i) {
        if (n % i == 0) {
            /* divisor d1 = i */
            seq_t term1;
            if (i == 1) {
                term1 = (seq_t)1;                 /* 1^(−1) treated as 1 */
            } else {
                /* exponent i-2 is non‑negative here */
                term1 = big_pow((seq_t)i, (seq_t)(i - 2));
            }
            sum += term1;

            /* divisor d2 = n / i, if distinct */
            idx_t d2 = n / i;
            if (d2 != i) {
                seq_t term2;
                if (d2 == 1) {
                    term2 = (seq_t)1;
                } else {
                    term2 = big_pow((seq_t)d2, (seq_t)(d2 - 2));
                }
                sum += term2;
            }
        }
    }

    return sum;
}
