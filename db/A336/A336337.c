/*
 * A336337  Total number of records over all length n ternary words (words on
 *          alphabet {0,1,2}).
 * https://oeis.org/A336337
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 11/2*3^(n-1) - 2^n - 1/2 (for n > 0), a(0)=0
 * Range:     u8 n <= 3, u16 n <= 8, u32 n <= 19, u64 n <= 27, u128 n <= 27,
 *            big any n
 * Helpers:   pow2, pow3
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computation uses the helper functions pow2 and pow3 from the utils library */

#include "../utils/pow2.h"
#include "../utils/pow3.h"

seq_t a(idx_t n)
{
    if (n == 0u) return 0;

    /* 11 * 3^(n-1) */
    seq_t term = pow3(n - 1);
    term = term * (seq_t)11;

    /* 2^(n+1) */
    seq_t pow2n1 = pow2(n + 1);

    /* (11*3^(n-1) - 2^(n+1) - 1) / 2 */
    seq_t result = term - pow2n1 - (seq_t)1;
    result = result / (seq_t)2;

    return result;
}
