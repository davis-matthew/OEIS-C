/*
 * A262389  Numbers whose last digit is composite.
 * https://oeis.org/A262389
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base, easy
 * Formula:   not stated
 * Range:     u8 n <= 60, u16 n <= 60, u32 n <= 60, u64 n <= 60, u128 n <= 60,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A262389: Numbers whose last digit is composite.
   Derived formula:
      let m = floor((n-1)/4),  r = (n-1) mod 4
      a(n) = 10*m + d[r],  where d = {4,6,8,9}.
*/
seq_t a(idx_t n)
{
    /* Values for the allowed last digits, in order of increasing n. */
    static const seq_t d[4] = {4, 6, 8, 9};

    /* Guard for undefined behaviour when n < 1; sequence offset is 1. */
    if (n == 0)
        return (seq_t)0;

    /* Compute the decade index and remainder. */
    idx_t m = (n - 1) / 4;
    idx_t r = (n - 1) % 4;

    /* Compose the result. All operations are performed in seq_t to avoid
       overflow in the target type (which may be 8, 16, 32, 64 or 128 bits,
       or arbitrary precision). */
    seq_t result = (seq_t)10 * (seq_t)m + d[r];

    return result;
}
