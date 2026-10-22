/*
 * A355689  Dirichlet inverse of A166486, characteristic function of numbers
 *          that are not multiples of 4.
 * https://oeis.org/A355689
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  sign, mult
 * Formula:   not stated
 * Range:     i8 n <= 9137, i16 n <= 9137, i32 n <= 9137, i64 n <= 9137,
 *            i128 n <= 9137, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A355689 – Dirichlet inverse of A166486 (characteristic
 * function of numbers that are not multiples of 4).
 * The sequence is multiplicative:
 *   a(2^e)   = (-1)^e,
 *   a(p^e)   = -1 if e == 1  (p odd prime),
 *   a(p^e)   =  0 if e > 1.
 * Hence for n = 2^e2 * ∏ p_i^e_i  (p_i odd),
 *   a(n) = (-1)^(e2 + sum_{e_i==1} 1)   if all e_i <= 1,
 *   a(n) = 0                              otherwise.
 */

#include <stdint.h>   /* for uint64_t used by idx_t, if needed */

seq_t a(idx_t n)
{
    seq_t sign = 1;         /* start with +1 */
    idx_t m = n;

    /* Extract powers of two, flipping the sign each time. */
    while ((m & 1) == 0) {
        m >>= 1;
        sign = -sign;
    }

    /* If the remaining part is 1, we are done. */
    if (m == 1)
        return sign;

    /* Factor the odd part by trial division. */
    for (idx_t i = 3; i <= m / i; i += 2) {
        if (m % i == 0) {
            /* Count how many times i divides m. */
            unsigned e = 0;
            while (m % i == 0) {
                m /= i;
                ++e;
            }
            /* Exponent greater than 1 causes the term to vanish. */
            if (e > 1)
                return (seq_t)0;
            /* Exactly one factor of i flips the sign. */
            sign = -sign;
        }
    }

    /* If what remains after division is a prime > sqrt(original m). */
    if (m > 1)
        sign = -sign;

    return sign;
}
