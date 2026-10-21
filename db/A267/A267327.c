/*
 * A267327  a(n) = decimal expansion of (n-1) prepended n times to the decimal
 *          expansion of n (n > 0).
 * https://oeis.org/A267327
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy, base
 * Formula:   a(n) = concatenation of (n-1) repeated n times followed by n
 * Range:     u8 n <= 2, u16 n <= 4, u32 n <= 8, u64 n <= 10, u128 n <= 16,
 *            big any n
 * Helpers:   digits_of, pow10
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/pow10.h"
#include "../utils/digits_of.h"

seq_t a(idx_t n)
{
    /* The sequence is defined only for n >= 1.  For a(n) with n == 0 we
       simply return 0, though this case will not appear in the tests. */
    if (n == 0)
        return (seq_t)0;

    /* Number of decimal digits of n and of (n-1). */
    idx_t len_n  = digits_of(n);
    idx_t len_nm1 = digits_of(n - 1);

    /* Precompute powers of ten.  pow10(e) returns 10^e as a seq_t. */
    seq_t pow_len_nm1 = pow10(len_nm1);
    seq_t pow_len_n   = pow10(len_n);

    /* Build the concatenation.  We keep a running value 'x' that
       represents the digits already produced.  For each repetition
       of (n-1) we shift left by len_nm1 digits (multiply by 10^len_nm1)
       and add the digit value (n-1).  After the loop we append n
       in the same manner. */
    seq_t result = 0;
    for (idx_t i = 0; i < n; ++i)
    {
        result = result * pow_len_nm1 + (seq_t)(n - 1);
    }
    result = result * pow_len_n + (seq_t)n;
    return result;
}
