/*
 * A351565  Odd part of Kimberling's paraphrase of the binary number system:
 *          a(n) = A000265(A003602(n)).
 * https://oeis.org/A351565
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = odd_part(1 + odd_part(n)) // odd_part(n) = largest odd
 *            divisor of n
 * Range:     u8 n <= 93, u16 n <= 9966, u32 n <= 9966, u64 n <= 9966,
 *            u128 n <= 9966, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n) {
    // seq_t may be of arbitrary size; operations are defined for all integer types.
    // n >= 1 by OEIS offset
    seq_t x = (seq_t)n;

    /* first odd part: remove all factors of 2 from n */
    while ((x & 1) == 0)
        x >>= 1;

    /* add 1, then remove all factors of 2 again */
    x = x + 1;
    while ((x & 1) == 0)
        x >>= 1;

    return x;
}
