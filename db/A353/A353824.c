/*
 * A353824  The positions of nonzero digits in the ternary expansions of n and
 *          a(n) are the same, and the k-th leftmost nonzero digit in a(n)
 *          equals modulo 3 the product of the k leftmost nonzero digits in n.
 * https://oeis.org/A353824
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) = prefix product of nonzero ternary digits modulo 3
 *            (positions of zeros unchanged)
 * Range:     u8 n <= 255, u16 n <= 6561, u32 n <= 6561, u64 n <= 6561,
 *            u128 n <= 6561, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n)
{
    if (n == 0)
        return 0;

    /* Store ternary digits of n (least significant first). */
    idx_t tmp = n;
    uint8_t digits[64];
    int count = 0;
    while (tmp > 0) {
        digits[count++] = (uint8_t)(tmp % 3);
        tmp /= 3;
    }

    seq_t result = 0;
    int prod = 1;                      /* Current prefix product modulo 3. */

    for (int i = count - 1; i >= 0; --i) {
        uint8_t d = digits[i];
        result = result * 3;
        uint8_t new_digit;
        if (d == 0) {
            new_digit = 0;
        } else {
            prod = (prod * d) % 3;
            new_digit = (uint8_t)prod;
        }
        result += new_digit;
    }
    return result;
}
