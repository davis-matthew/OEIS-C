/*
 * A266193  Decrement by 1 all maximal digits in factorial base representation
 *          of n and then shift it one digit right.
 * https://oeis.org/A266193
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 n <= 1499, u16 n <= 9998, u32 n <= 9998, u64 n <= 9998,
 *            u128 n <= 9998, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Implements A266193:
 * For each n: take the factorial‑base representation of n,
 * subtract 1 from every digit that equals its (1‑based) position,
 * shift the digits one place to the right (drop the least significant digit),
 * and interpret the result as a factorial‑base number.
 * The value in decimal is returned.
 */

#include <stddef.h>   /* for size_t */

seq_t a(idx_t n)
{
    /* maximum length needed for factorial base of 64‑bit integer is 20,
       array size 64 is more than sufficient */
    seq_t digits[64];
    idx_t  m = n;
    size_t pos = 1;

    /* convert n to factorial base, least significant digit first (digits[1] is position 1) */
    while (m > 0) {
        seq_t rem = (seq_t)(m % ((idx_t)(pos + 1)));
        digits[pos] = rem;
        m = m / ((idx_t)(pos + 1));
        ++pos;
    }

    /* n == 0 has no digits */
    if (pos == 1) {
        return (seq_t)0;
    }

    size_t k = pos - 1;   /* number of digits */

    /* subtract 1 from every maximal digit (digit == its position) */
    for (pos = 1; pos <= k; ++pos) {
        if (digits[pos] == (seq_t)pos) {
            digits[pos]--;   /* decrement in place */
        }
    }

    /* compute the shifted value:
       new least significant digit comes from old position 2,
       factorial contribution of position j is j! (j = 1..k-1) */
    seq_t result = (seq_t)0;
    seq_t fact   = (seq_t)1;   /* fact = 1! initially */

    for (pos = 2; pos <= k; ++pos) {
        fact = fact * (idx_t)(pos - 1);      /* fact = (pos-1)! */
        result = result + digits[pos] * fact;
    }

    return result;
}
