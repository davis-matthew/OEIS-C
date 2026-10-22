/*
 * A349415  Number of ways an n-set can be written as the union of 2 sets each
 *          with 4 or more elements and whose intersection contains exactly 3
 *          elements.
 * https://oeis.org/A349415
 *
 * Offset:    5    (a(5) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = binomial(n,3) * (2^(n-4) - 1)
 * Range:     u8 n <= 7, u16 n <= 12, u32 n <= 24, u64 n <= 32, u128 n <= 32,
 *            big any n
 * Helpers:   binomial, pow2
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Returned only for n >= 5; for smaller n the value is undefined.
   The function is O(1) per call. */
#include "../utils/binomial.h"
#include "../utils/pow2.h"

seq_t a(idx_t n)
{
    /* For n < 5 the sequence is not defined; return 0 */
    if (n < 5) return 0;

    seq_t choose = binomial(n, 3);      /* C(n,3) */
    seq_t powval = pow2(n - 4);         /* 2^(n-4) */
    return choose * (powval - 1);
}
