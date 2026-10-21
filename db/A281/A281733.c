/*
 * A281733  Positive integers T_i such that Sum_{k >= 0} (S_k * x^(2*k+1)) +
 *          1/24 - Sum_{k >= 1} (T_k * x^(2*k)) = (cos((2/3) * arccos(6 *
 *          sqrt(3) * x)))/12 for all real x with |x| <= 1/(6 * sqrt(3)),
 *          where S_k = A176898(k).
 * https://oeis.org/A281733
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 16^(n-1) * binomial(3*n-2, 2*n-1) / n
 * Range:     u8 n <= 2, u16 n <= 3, u32 n <= 6, u64 n <= 11, u128 n <= 20,
 *            big any n
 * Helpers:   binomial, pow_uint
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Utility function to compute the n-th term of OEIS sequence A281733.
   The function works for any seq_t defined by the build system
   (unsigned integer of any supported bit-width or an arbitrary-precision type).  */
#include "../utils/binomial.h"    /* seq_t binomial(idx_t n, idx_t k);          */
#include "../utils/pow_uint.h"    /* seq_t pow_uint(uint64_t base, idx_t exp); */

seq_t a(idx_t n)
{
    /* n >= 1.  For safety, return 0 for nonpositive input. */
    if (n == 0)
        return 0;

    /* Compute the central binomial coefficient part. */
    idx_t base   = 3*n - 2;          /* 3*n - 2   */
    idx_t choose = 2*n - 1;          /* 2*n - 1   */
    seq_t bin    = binomial(base, choose);

    /* Reduce the size of the intermediate result by dividing by n first.
       The division is exact by definition of the sequence. */
    seq_t bin_div = bin / (seq_t)n;

    /* Compute 16^(n-1).  pow_uint returns an object of type seq_t
       and handles large intermediate values gracefully. */
    seq_t pow16 = pow_uint(16, n-1);

    /* Final multiplication.  For arbitrary-precision types this
       may allocate a few digits; for fixed-width types it works
       as long as the result fits in the type. */
    return pow16 * bin_div;
}
