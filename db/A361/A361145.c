/*
 * A361145  Number of downwards peaks in row 2*n-1 of the Sierpinski triangle.
 * https://oeis.org/A361145
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy, look
 * Formula:   a(n) = 2^(wt(n-1)+1) - 2^(wt(n)-1), where wt(x) is the Hamming
 *            weight of x
 * Range:     u8 n <= 255, u16 n <= 8946, u32 n <= 8946, u64 n <= 8946,
 *            u128 n <= 8946, big any n
 * Helpers:   pow2
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* (OEIS A361145) */

#include <stdint.h>          /* For uint64_t, __builtin_popcountll   */
#include "../utils/pow2.h"   /* Provides seq_t pow2(idx_t exp)       */

seq_t a(idx_t n)
{
    /* Guard against out‑of‑range inputs – the sequence is only defined for n ≥ 1 */
    if (n == 0)
        return 0;

    /* Compute the Hamming weights of n-1 and n using the GCC builtin */
    int w_prev = __builtin_popcountll((uint64_t)(n - 1));
    int w_curr = __builtin_popcountll((uint64_t)n);

    /* Convert the exponents to idx_t before calling pow2 */
    seq_t term1 = pow2((idx_t)(w_prev + 1));   /* 2^(wt(n-1)+1)   */
    seq_t term2 = pow2((idx_t)(w_curr - 1));   /* 2^(wt(n)-1)     */

    /* The formula guarantees a non‑negative result that fits in seq_t */
    return term1 - term2;
}
