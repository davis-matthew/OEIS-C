/*
 * A267318  Continued fraction expansion of e^(1/5).
 * https://oeis.org/A267318
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, cofr, easy
 * Formula:   a(n) = 1 + (3 + 10*floor(n/3)) * (1 - ((n-1)^2 mod 3))
 * Range:     u8 n <= 78, u16 n <= 81, u32 n <= 81, u64 n <= 81, u128 n <= 81,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* For unsigned arithmetic, the factor (1 - ((n-1)^2 mod 3)) is 1 exactly when
   n % 3 == 1, and 0 otherwise. */
seq_t a(idx_t n)
{
    /* When n is congruent to 1 modulo 3, the term is 1 + (3 + 10*floor(n/3));
       otherwise it is just 1. */
    if ((n % 3) == 1) {
        seq_t coeff = (seq_t)3 + (seq_t)10 * (seq_t)(n / 3);
        return (seq_t)1 + coeff;
    } else {
        return (seq_t)1;
    }
}
