/*
 * A269496  Number of length-5 0..n arrays with no repeated value differing
 *          from the previous repeated value by one.
 * https://oeis.org/A269496
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 2, u16 n <= 8, u32 n <= 83, u64 n <= 210, u128 n <= 210,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* OEIS sequence A269496
   Formula: a(n) = n^5 + 5*n^4 + 10*n^3 + 4*n^2 + 3*n + 1
   (valid for all n ≥ 1)
*/

seq_t a(idx_t n)
{
    /* Convert the index to seq_t once. */
    seq_t n_ = (seq_t)n;

    /* Compute powers of n incrementally, using seq_t arithmetic
       which works for all supported integer types (fixed-width or
       arbitrary-precision). */
    seq_t n2 = n_ * n_;
    seq_t n3 = n2 * n_;
    seq_t n4 = n3 * n_;
    seq_t n5 = n4 * n_;

    /* Evaluate the polynomial directly.  All multipliers are cast to
       seq_t so that the compiler performs the operation in the
       appropriate integer type. */
    seq_t result = n5
                  + n4 * (seq_t)5
                  + n3 * (seq_t)10
                  + n2 * (seq_t)4
                  + n_ * (seq_t)3
                  + (seq_t)1;

    return result;
}
